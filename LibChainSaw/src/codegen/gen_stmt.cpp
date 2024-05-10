#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/FunctionRef.hpp>
#include <csaw/codegen/ValueRef.hpp>
#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Stmt.hpp>
#include <llvm/IR/Verifier.h>

void csaw::Builder::Gen(const StatementPtr& ptr)
{
    if (!ptr)
        CSAW_MESSAGE(true, "statement must not be null");

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
        (void)Gen(p);
        return;
    }

    CSAW_MESSAGE(true, "in line " + std::to_string(ptr->Line) + ": code generation for statement is not implemented");
}

void csaw::Builder::Gen(const ScopeStatement& statement)
{
    for (const auto& ptr : statement.Body)
        Gen(ptr);
}

void csaw::Builder::Gen(const ForStatement& statement)
{
    const auto hdr_block = llvm::BasicBlock::Create(*m_Context, "hdr", m_Builder->GetInsertBlock()->getParent());
    const auto loop_block = llvm::BasicBlock::Create(*m_Context, "loop", m_Builder->GetInsertBlock()->getParent());
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end", m_Builder->GetInsertBlock()->getParent());

    if (statement.Pre) Gen(statement.Pre);
    m_Builder->CreateBr(hdr_block);
    m_Builder->SetInsertPoint(hdr_block);
    const auto condition = statement.Condition ? Gen(statement.Condition) : ValueRef::Constant(this, m_Builder->getInt1(true), Type::GetInt1());
    m_Builder->CreateCondBr(condition.Load().GetValue(), loop_block, end_block);
    m_Builder->SetInsertPoint(loop_block);
    if (statement.Body) Gen(statement.Body);
    if (statement.Loop) Gen(statement.Loop);
    m_Builder->CreateBr(hdr_block);
    m_Builder->SetInsertPoint(end_block);
}

void csaw::Builder::Gen(const FunctionStatement& statement)
{
    const bool has_ptr = statement.IsConstructor || statement.Callee;

    std::vector<TypePtr> args(statement.Args.size());
    std::vector<llvm::Type*> arg_types(statement.Args.size());
    for (size_t i = 0; i < args.size(); ++i) arg_types[i] = Gen(args[i] = statement.Args[i].second);
    if (has_ptr) arg_types.insert(arg_types.begin(), m_Builder->getPtrTy());

    auto& ref = GetOrCreateFunction(statement.Name, statement.Callee, statement.Result, args, statement.IsConstructor, statement.IsVarArg);
    if (!ref.Function)
    {
        const auto result_type = statement.IsConstructor ? Gen(Type::GetVoid()) : Gen(statement.Result);
        const auto function_type = llvm::FunctionType::get(result_type, arg_types, statement.IsVarArg);
        ref.Function = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, statement.Name, *m_Module);

        int i = has_ptr ? -1 : 0;
        for (auto& arg : ref.Function->args())
        {
            arg.setName(i < 0 ? "me" : statement.Args[i].first);
            ++i;
        }
    }

    if (!statement.Body)
        return;

    if (!ref.Function->empty())
        CSAW_MESSAGE(true, "in line " + std::to_string(statement.Line) + ": function is already implemented");

    const auto entry_block = llvm::BasicBlock::Create(*m_Context, "entry", ref.Function);
    m_Builder->SetInsertPoint(entry_block);

    m_Values.clear();
    int i = has_ptr ? -1 : 0;
    for (auto& arg : ref.Function->args())
    {
        if (i < 0)
        {
            const auto type = statement.IsConstructor ? Type::Get(statement.Name) : statement.Callee;
            m_Values[arg.getName().str()] = ValueRef::Allocate(this, &arg, PointerType::Get(type));
        }
        else
        {
            m_Values[arg.getName().str()] = ValueRef::Allocate(this, &arg, statement.Args[i].second);
        }
        ++i;
    }

    if (const auto scope_statement = std::dynamic_pointer_cast<ScopeStatement>(statement.Body))
    {
        Gen(scope_statement);

        if (ref.Function->getFunctionType()->getReturnType()->isVoidTy())
            for (const auto& block : *ref.Function)
                if (!block.getTerminator())
                    m_Builder->CreateRetVoid();
    }
    else if (const auto expression = std::dynamic_pointer_cast<Expression>(statement.Body))
    {
        const auto result = Gen(expression);
        if (ref.Function->getFunctionType()->getReturnType()->isVoidTy())
            m_Builder->CreateRetVoid();
        else m_Builder->CreateRet(result.Load().GetValue());
    }
    else
    {
        Gen(statement.Body);
        m_Builder->CreateRetVoid();
    }

    m_Builder->SetInsertPoint(static_cast<llvm::BasicBlock*>(nullptr));

    if (verifyFunction(*ref.Function, &llvm::errs()))
    {
        ref.Function->viewCFG();
        CSAW_MESSAGE(true, "in line " + std::to_string(statement.Line) + ": failed to verify function");
    }

    m_FPM->run(*ref.Function, *m_FAM);
}

void csaw::Builder::Gen(const IfStatement& statement)
{
    const auto condition = Gen(statement.Condition);

    const auto true_block = llvm::BasicBlock::Create(*m_Context, "true", m_Builder->GetInsertBlock()->getParent());
    const auto false_block = llvm::BasicBlock::Create(*m_Context, "false", m_Builder->GetInsertBlock()->getParent());
    const auto end_block = statement.False ? llvm::BasicBlock::Create(*m_Context, "end", m_Builder->GetInsertBlock()->getParent()) : false_block;

    bool need_end = false;

    m_Builder->CreateCondBr(condition.Load().GetValue(), true_block, false_block);

    m_Builder->SetInsertPoint(true_block);
    Gen(statement.True);
    if (!true_block->getTerminator())
    {
        m_Builder->CreateBr(end_block);
        need_end = true;
    }

    m_Builder->SetInsertPoint(false_block);
    if (statement.False)
    {
        Gen(statement.False);
        if (!false_block->getTerminator())
        {
            m_Builder->CreateBr(end_block);
            need_end = true;
        }
    }
    else need_end = true;

    if (!need_end)
        end_block->eraseFromParent();
    else
        m_Builder->SetInsertPoint(end_block);
}

void csaw::Builder::Gen(const RetStatement& statement)
{
    if (!statement.Value)
    {
        m_Builder->CreateRetVoid();
        return;
    }

    const auto value = Gen(statement.Value);
    m_Builder->CreateRet(value.Load().GetValue());
}

void csaw::Builder::Gen(const DefStatement& statement) const
{
    if (statement.Origin)
    {
        Type::Alias(statement.Name, statement.Origin);
        return;
    }

    StructType::Get(statement.Name, statement.Elements);

    if (statement.Elements.empty())
    {
        if (!llvm::StructType::getTypeByName(*m_Context, statement.Name))
            llvm::StructType::create(*m_Context, statement.Name);
        return;
    }

    const auto type = llvm::StructType::getTypeByName(*m_Context, statement.Name);
    if (type && !type->isEmptyTy())
        CSAW_MESSAGE(true, "in line " + std::to_string(statement.Line) + ": struct is already implemented");

    std::vector<llvm::Type*> elements(statement.Elements.size());
    for (size_t i = 0; i < elements.size(); ++i)
        elements[i] = Gen(statement.Elements[i].second);

    if (!type)
        llvm::StructType::create(*m_Context, elements, statement.Name);
    else
        type->setBody(elements);
}

void csaw::Builder::Gen(const VariableStatement& statement)
{
    if (IsGlobal())
    {
        llvm::Constant* global_initializer = nullptr;
        if (statement.Value)
        {
            const auto initializer = Gen(statement.Value);
            global_initializer = llvm::dyn_cast<llvm::Constant>(initializer.Load().GetValue());
        }

        const auto type = Gen(statement.Type);
        const auto value = new llvm::GlobalVariable(*m_Module, type, false, llvm::GlobalValue::InternalLinkage, global_initializer, statement.Name);

        m_GlobalValues[statement.Name] = ValueRef::Pointer(this, value, statement.Type);

        return;
    }

    m_Values[statement.Name] = ValueRef::Allocate(this, statement.Value ? Gen(statement.Value).Load().GetValue() : nullptr, statement.Type);
}

void csaw::Builder::Gen(const WhileStatement& statement)
{
    const auto hdr_block = llvm::BasicBlock::Create(*m_Context, "hdr", m_Builder->GetInsertBlock()->getParent());
    const auto loop_block = llvm::BasicBlock::Create(*m_Context, "loop", m_Builder->GetInsertBlock()->getParent());
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end", m_Builder->GetInsertBlock()->getParent());

    m_Builder->CreateBr(hdr_block);
    m_Builder->SetInsertPoint(hdr_block);
    const auto condition = Gen(statement.Condition);
    m_Builder->CreateCondBr(condition.Load().GetValue(), loop_block, end_block);
    m_Builder->SetInsertPoint(loop_block);
    if (statement.Body) Gen(statement.Body);
    m_Builder->CreateBr(hdr_block);
    m_Builder->SetInsertPoint(end_block);
}
