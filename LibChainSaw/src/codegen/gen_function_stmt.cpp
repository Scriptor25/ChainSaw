#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Stmt.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>
#include <llvm/IR/Verifier.h>

void csaw::Builder::Gen(const FunctionStatement& statement)
{
    const bool has_ptr = statement.IsConstructor() || statement.Parent;

    if (statement.IsConstructor())
        if (const auto type = Gen(Type::Get(statement.Name)); !type)
            return ThrowErrorStmt(statement, false, "Failed to generate type %s: %s", statement.Name.c_str(), type.Msg().c_str());

    std::vector<TypePtr> args(statement.Args.size());
    std::vector<llvm::Type*> arg_types(statement.Args.size());
    for (size_t i = 0; i < args.size(); ++i)
    {
        const auto arg = Gen(args[i] = statement.Args[i].Type);
        if (!arg)
            return ThrowErrorStmt(statement, false, "Failed to generate type %s: %s", statement.Args[i].Type->Name.c_str(), arg.Msg().c_str());
        arg_types[i] = arg.Get();
    }

    if (has_ptr)
        arg_types.insert(arg_types.begin(), GetBuilder().getPtrTy());

    auto [function, signature] = FindFunction(statement.Name, statement.Parent, args);

    if (!function)
    {
        signature.Result = statement.Result;
        signature.IsVarargs = statement.IsVarArgs;
        signature.IsC = std::ranges::find(statement.Mods, "c") != statement.Mods.end();

        const auto result_type = signature.IsConstructor() ? Gen(Type::GetVoid()) : Gen(statement.Result);
        if (!result_type)
            return ThrowErrorStmt(statement, false, "Failed to generate type %s: %s", statement.Result->Name.c_str(), result_type.Msg().c_str());

        const auto function_type = llvm::FunctionType::get(result_type.Get(), arg_types, statement.IsVarArgs);
        function = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, signature.Mangle(), GetModule());

        m_Signatures[function] = signature;
    }

    if (!statement.Body)
        return;

    if (!function->empty())
        return ThrowErrorStmt(statement, false, "Function is already implemented");

    const auto entry_block = llvm::BasicBlock::Create(GetContext(), "entry", function);
    const auto bkp_block = GetBuilder().GetInsertBlock();
    GetBuilder().SetInsertPoint(entry_block);
    PushScopeStack();

    int i = has_ptr ? -1 : 0;
    for (auto& arg : function->args())
    {
        const auto name = i < 0 ? "me" : statement.Args[i].Name;
        if (name.empty())
            continue;

        arg.setName(name);
        if (i < 0)
        {
            const auto type = signature.IsConstructor() ? Type::Get(statement.Name) : statement.Parent;
            const auto alloc = LValue::AllocateAndStore(this, PointerType::Get(type), &arg);
            if (!alloc)
            {
                PopScopeStack();
                GetBuilder().SetInsertPoint(bkp_block);
                m_Signatures.erase(function);
                function->eraseFromParent();
                return ThrowErrorStmt(statement, false, "Failed to allocate: %s", alloc.Msg().c_str());
            }
            m_Values["me"] = alloc.Get();
        }
        else
        {
            const auto alloc = LValue::AllocateAndStore(this, statement.Args[i].Type, &arg);
            if (!alloc)
            {
                PopScopeStack();
                GetBuilder().SetInsertPoint(bkp_block);
                m_Signatures.erase(function);
                function->eraseFromParent();
                return ThrowErrorStmt(statement, false, "Failed to allocate: %s", alloc.Msg().c_str());
            }
            m_Values[name] = alloc.Get();
        }

        ++i;
    }

    if (const auto scope_statement = std::dynamic_pointer_cast<ScopeStatement>(statement.Body))
    {
        Gen(scope_statement);

        if (function->getFunctionType()->getReturnType()->isVoidTy())
            for (const auto& block : *function)
                if (!block.getTerminator())
                    GetBuilder().CreateRetVoid();
    }
    else if (const auto expression = std::dynamic_pointer_cast<Expression>(statement.Body))
    {
        const auto result = Gen(expression);
        if (!result)
        {
            PopScopeStack();
            GetBuilder().SetInsertPoint(bkp_block);
            m_Signatures.erase(function);
            function->eraseFromParent();
            return;
        }

        if (function->getFunctionType()->getReturnType()->isVoidTy())
            GetBuilder().CreateRetVoid();
        else
        {
            const auto cast = Cast(result, signature.Result);
            if (!cast)
            {
                PopScopeStack();
                GetBuilder().SetInsertPoint(bkp_block);
                m_Signatures.erase(function);
                function->eraseFromParent();
                return ThrowErrorStmt(statement, false, "Failed to cast: %s", cast.Msg().c_str());
            }
            GetBuilder().CreateRet(cast.Get()->GetValue());
        }
    }
    else
    {
        Gen(statement.Body);
        GetBuilder().CreateRetVoid();
    }

    PopScopeStack();
    GetBuilder().SetInsertPoint(bkp_block);

    if (verifyFunction(*function, &llvm::errs()))
    {
        function->viewCFG();
        m_Signatures.erase(function);
        function->eraseFromParent();
        return ThrowErrorStmt(statement, false, "Failed to verify function");
    }

    m_FPM->run(*function, *m_FAM);
}
