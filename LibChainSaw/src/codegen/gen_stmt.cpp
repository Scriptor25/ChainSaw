#include <iostream>
#include <csaw/Error.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Signature.hpp>
#include <csaw/codegen/Value.hpp>
#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Stmt.hpp>
#include <llvm/IR/Verifier.h>

void csaw::Builder::Gen(const StatementPtr& ptr)
{
    if (!ptr)
        return;

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

    ThrowErrorStmt(*ptr, false, "Code generation is not implemented for this statement");
}

void csaw::Builder::Gen(const ScopeStatement& statement)
{
    PushScopeStack();
    for (const auto& ptr : statement.Body)
        Gen(ptr);
    PopScopeStack();
}

void csaw::Builder::Gen(const ForStatement& statement)
{
    const auto parent = m_Builder->GetInsertBlock()->getParent();
    const auto bkp_block = m_Builder->GetInsertBlock();

    const auto hdr_block = llvm::BasicBlock::Create(*m_Context, "hdr", parent);
    const auto loop_block = llvm::BasicBlock::Create(*m_Context, "loop");
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end");

    if (statement.Pre)
        Gen(statement.Pre);

    const auto br_inst = m_Builder->CreateBr(hdr_block);
    m_Builder->SetInsertPoint(hdr_block);

    const auto condition = statement.Condition ? Gen(statement.Condition) : RValue::Create(Type::GetInt1(), m_Builder->getInt1(true));
    if (!condition)
    {
        br_inst->eraseFromParent();
        hdr_block->eraseFromParent();
        m_Builder->SetInsertPoint(bkp_block);
        return;
    }

    m_Builder->CreateCondBr(condition->GetBoolValue(this), loop_block, end_block);

    loop_block->insertInto(parent);
    m_Builder->SetInsertPoint(loop_block);

    if (statement.Body)
        Gen(statement.Body);

    if (statement.Loop)
        Gen(statement.Loop);

    m_Builder->CreateBr(hdr_block);

    end_block->insertInto(parent);
    m_Builder->SetInsertPoint(end_block);
}

void csaw::Builder::Gen(const IfStatement& statement)
{
    const auto parent = m_Builder->GetInsertBlock()->getParent();

    auto true_block = llvm::BasicBlock::Create(*m_Context, "true");
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end");
    auto false_block = statement.False ? llvm::BasicBlock::Create(*m_Context, "false") : end_block;

    const auto condition = Gen(statement.Condition);
    if (!condition)
        return;

    bool need_end = false;

    m_Builder->CreateCondBr(condition->GetBoolValue(this), true_block, false_block);

    true_block->insertInto(parent);
    m_Builder->SetInsertPoint(true_block);
    Gen(statement.True);
    true_block = m_Builder->GetInsertBlock();
    if (!true_block->getTerminator())
    {
        m_Builder->CreateBr(end_block);
        need_end = true;
    }

    false_block->insertInto(parent);
    m_Builder->SetInsertPoint(false_block);
    if (statement.False)
    {
        Gen(statement.False);
        false_block = m_Builder->GetInsertBlock();
        if (!false_block->getTerminator())
        {
            m_Builder->CreateBr(end_block);
            need_end = true;
        }
    }
    else need_end = false;

    if (need_end)
    {
        end_block->insertInto(parent);
        m_Builder->SetInsertPoint(end_block);
    }
}

void csaw::Builder::Gen(const RetStatement& statement)
{
    if (!statement.Value)
    {
        m_Builder->CreateRetVoid();
        return;
    }

    const auto type = FromLLVM(m_Builder->getCurrentFunctionReturnType());
    if (!type)
        return ThrowErrorStmt(statement, false, "Failed to determine type from llvm type: %s", type.Msg().c_str());

    const auto value = Gen(statement.Value);
    if (!value)
        return;

    const auto cast = Cast(value, type.Get());
    if (!cast)
        return ThrowErrorStmt(statement, false, "Failed to cast: %s", cast.Msg().c_str());

    m_Builder->CreateRet(cast.Get()->GetValue());
}

void csaw::Builder::Gen(const DefStatement& statement) const
{
    if (statement.Origin)
    {
        Type::Alias(statement.Name, statement.Origin);
        return;
    }

    if (statement.Elements.empty())
    {
        if (!llvm::StructType::getTypeByName(*m_Context, statement.Name))
            llvm::StructType::create(*m_Context, statement.Name);
        return;
    }

    const auto type = llvm::StructType::getTypeByName(*m_Context, statement.Name);
    if (type && !type->isEmptyTy())
        return ThrowErrorStmt(statement, false, "Struct type is already implemented");

    std::vector<llvm::Type*> elements(statement.Elements.size());
    for (size_t i = 0; i < elements.size(); ++i)
    {
        const auto ety = Gen(statement.Elements[i].second);
        if (!ety)
            return ThrowErrorStmt(statement, false, "Failed to generate type %s: %s", statement.Elements[i].second->Name.c_str(), ety.Msg().c_str());

        elements[i] = ety.Get();
    }

    if (!type)
        llvm::StructType::create(*m_Context, elements, statement.Name);
    else
        type->setBody(elements);
}

void csaw::Builder::Gen(const VariableStatement& statement)
{
    const auto type = Gen(statement.Type);
    if (!type)
        return ThrowErrorStmt(statement, false, "Failed to generate type %s: %s", statement.Type->Name.c_str(), type.Msg().c_str());

    ValuePtr initializer;
    if (statement.Value) initializer = Gen(statement.Value);
    else if (const auto [signature, function] = FindFunction(statement.Type->Name, nullptr, {}); function && signature.IsConstructor())
    {
        const auto alloc = LValue::Allocate(this, Type::Get(signature.Name));
        if (!alloc)
            return ThrowErrorStmt(statement, false, "Failed to allocate: %s", alloc.Msg().c_str());

        initializer = alloc.Get();
        m_Builder->CreateCall(function->getFunctionType(), function, {alloc.Get()->GetPointer()});
    }

    if (initializer)
    {
        const auto cast = Cast(initializer, statement.Type);
        if (!cast)
            return ThrowErrorStmt(statement, false, "Failed to cast: %s", cast.Msg().c_str());

        initializer = cast.Get();
    }

    if (m_ScopeStack.empty())
    {
        llvm::Constant* global_initializer = nullptr;
        if (initializer)
            global_initializer = llvm::dyn_cast<llvm::Constant>(initializer->GetValue());

        const bool is_initialized = global_initializer;
        if (!is_initialized)
            global_initializer = llvm::Constant::getNullValue(type.Get());

        const auto pointer = new llvm::GlobalVariable(*m_Module, type.Get(), false, llvm::GlobalValue::InternalLinkage, global_initializer, statement.Name);
        const auto ref = LValue::Direct(this, statement.Type, pointer);

        if (!is_initialized && initializer)
            ref->StoreValue(initializer->GetValue());

        m_Values[statement.Name] = ref;
        return;
    }

    const auto alloc = LValue::AllocateAndStore(this, statement.Type, initializer ? initializer->GetValue() : llvm::Constant::getNullValue(type.Get()));
    if (!alloc)
        return ThrowErrorStmt(statement, false, "Failed to allocate: %s", alloc.Msg().c_str());

    m_Values[statement.Name] = alloc.Get();
}

void csaw::Builder::Gen(const WhileStatement& statement)
{
    const auto parent = m_Builder->GetInsertBlock()->getParent();
    const auto bkp_block = m_Builder->GetInsertBlock();

    const auto hdr_block = llvm::BasicBlock::Create(*m_Context, "hdr", parent);
    const auto loop_block = llvm::BasicBlock::Create(*m_Context, "loop");
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end");

    const auto br_inst = m_Builder->CreateBr(hdr_block);

    m_Builder->SetInsertPoint(hdr_block);
    const auto condition = Gen(statement.Condition);
    if (!condition)
    {
        br_inst->eraseFromParent();
        hdr_block->eraseFromParent();
        m_Builder->SetInsertPoint(bkp_block);
        return;
    }
    m_Builder->CreateCondBr(condition->GetBoolValue(this), loop_block, end_block);

    loop_block->insertInto(parent);
    m_Builder->SetInsertPoint(loop_block);
    if (statement.Body) Gen(statement.Body);
    m_Builder->CreateBr(hdr_block);

    end_block->insertInto(parent);
    m_Builder->SetInsertPoint(end_block);
}
