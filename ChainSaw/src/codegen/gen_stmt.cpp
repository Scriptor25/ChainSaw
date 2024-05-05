#include <csaw/codegen/Builder.hpp>
#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Stmt.hpp>
#include <llvm/IR/Verifier.h>

void csaw::Builder::Gen(const StatementPtr& ptr)
{
    if (const auto p = std::dynamic_pointer_cast<ScopeStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<ForStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<FunctionStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<IfStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<RetStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<DefStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<VariableStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<WhileStatement>(ptr))
        return Gen(*p);

    if (const auto p = std::dynamic_pointer_cast<Expression>(ptr))
    {
        Gen(p);
        return;
    }

    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const ScopeStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const ForStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const FunctionStatement& statement)
{
    std::vector<TypePtr> args(statement.Args.size());
    std::vector<llvm::Type*> arg_types(statement.Args.size() + 1);
    arg_types[0] = m_Builder->getPtrTy();
    for (size_t i = 0; i < args.size(); ++i) arg_types[i + 1] = Gen(args[i] = statement.Args[i].second);
    const auto function_name = GetFunctionName(statement.Name, statement.Constructor, statement.Callee, args, !statement.VarArg.empty());
    auto function = m_Module->getFunction(function_name);
    if (!function)
    {
        const auto result_type = Gen(statement.Result);
        const auto function_type = llvm::FunctionType::get(result_type, arg_types, !statement.VarArg.empty());
        function = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, function_name, *m_Module);

        int i = -1;
        for (auto& arg : function->args())
        {
            arg.setName(i < 0 ? "me" : statement.Args[i].first);
            ++i;
        }
    }

    if (!statement.Body)
        return;

    if (!function->empty())
        throw std::runtime_error("function already implemented");

    m_Values.clear();
    for (auto& arg : function->args())
        m_Values[arg.getName().str()] = &arg;

    const auto entry_block = llvm::BasicBlock::Create(*m_Context, "entry", function);
    m_Builder->SetInsertPoint(entry_block);

    if (const auto scope_statement = std::dynamic_pointer_cast<ScopeStatement>(statement.Body))
    {
        Gen(scope_statement);
        // TODO: check for missing ret
    }
    else if (const auto expression = std::dynamic_pointer_cast<Expression>(statement.Body))
    {
        const auto result = Gen(expression);
        m_Builder->CreateRet(result);
    }
    else
    {
        Gen(statement.Body);
        m_Builder->CreateRetVoid();
    }

    m_Builder->SetInsertPoint(static_cast<llvm::BasicBlock*>(nullptr));

    function->dump();

    if (verifyFunction(*function, &llvm::errs()))
        throw std::runtime_error("failed to verify function");
}

void csaw::Builder::Gen(const IfStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const RetStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const DefStatement& statement)
{
    if (statement.Origin)
    {
        Type::Alias(statement.Name, statement.Origin);
        return;
    }

    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const VariableStatement& statement)
{
    const auto type = Gen(statement.Type);
    const auto initializer = Gen(statement.Value);

    if (IsGlobal())
    {
        llvm::Constant* global_initializer = nullptr;
        if (initializer)
        {
            global_initializer = llvm::dyn_cast<llvm::Constant>(initializer);
            if (!global_initializer)
                throw std::runtime_error("not yet implemented");
        }

        (void)llvm::GlobalVariable(*m_Module, type, false, llvm::GlobalValue::InternalLinkage, global_initializer, statement.Name);
        return;
    }

    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const WhileStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}
