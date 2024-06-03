#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Stmt.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

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
        (void)Gen(p, nullptr);
        return;
    }

    ThrowErrorStmt(*ptr, false, "Code generation is not implemented for this statement");
}

void csaw::Builder::Gen(const DefStatement& statement)
{
    if (statement.Origin)
    {
        Type::Alias(statement.Name, statement.Origin);
        return;
    }

    if (statement.Elements.empty())
    {
        if (!llvm::StructType::getTypeByName(GetContext(), statement.Name))
            llvm::StructType::create(GetContext(), statement.Name);
        return;
    }

    const auto type = llvm::StructType::getTypeByName(GetContext(), statement.Name);
    if (type && !type->isEmptyTy())
        return ThrowErrorStmt(statement, false, "Struct type is already implemented");

    std::vector<llvm::Type*> elements(statement.Elements.size());
    for (size_t i = 0; i < elements.size(); ++i)
    {
        const auto ety = Gen(statement.Elements[i].Type);
        if (!ety)
            return ThrowErrorStmt(statement, false, "Failed to generate type %s: %s", statement.Elements[i].Type->Name.c_str(), ety.Msg().c_str());

        elements[i] = ety.Get();
    }

    if (!type)
        llvm::StructType::create(GetContext(), elements, statement.Name);
    else
        type->setBody(elements);
}

void csaw::Builder::Gen(const ForStatement& statement)
{
    const auto parent = GetBuilder().GetInsertBlock()->getParent();
    const auto bkp_block = GetBuilder().GetInsertBlock();

    const auto hdr_block = llvm::BasicBlock::Create(GetContext(), "hdr", parent);
    const auto loop_block = llvm::BasicBlock::Create(GetContext(), "loop");
    const auto end_block = llvm::BasicBlock::Create(GetContext(), "end");

    if (statement.Pre)
        Gen(statement.Pre);

    const auto br_inst = GetBuilder().CreateBr(hdr_block);
    GetBuilder().SetInsertPoint(hdr_block);

    const auto condition = statement.Condition ? Gen(statement.Condition, Type::GetInt1()) : RValue::Create(this, Type::GetInt1(), GetBuilder().getInt1(true));
    if (!condition)
    {
        br_inst->eraseFromParent();
        hdr_block->eraseFromParent();
        GetBuilder().SetInsertPoint(bkp_block);
        return;
    }

    GetBuilder().CreateCondBr(condition->GetBoolValue(), loop_block, end_block);

    loop_block->insertInto(parent);
    GetBuilder().SetInsertPoint(loop_block);

    if (statement.Body)
        Gen(statement.Body);

    if (statement.Loop)
        Gen(statement.Loop);

    GetBuilder().CreateBr(hdr_block);

    end_block->insertInto(parent);
    GetBuilder().SetInsertPoint(end_block);
}

void csaw::Builder::Gen(const IfStatement& statement)
{
    const auto parent = GetBuilder().GetInsertBlock()->getParent();

    auto true_block = llvm::BasicBlock::Create(GetContext(), "true");
    const auto end_block = llvm::BasicBlock::Create(GetContext(), "end");
    auto false_block = statement.False ? llvm::BasicBlock::Create(GetContext(), "false") : end_block;

    const auto condition = Gen(statement.Condition, Type::GetInt1());
    if (!condition)
        return;

    bool need_end = false;

    GetBuilder().CreateCondBr(condition->GetBoolValue(), true_block, false_block);

    true_block->insertInto(parent);
    GetBuilder().SetInsertPoint(true_block);
    Gen(statement.True);
    true_block = GetBuilder().GetInsertBlock();
    if (!true_block->getTerminator())
    {
        GetBuilder().CreateBr(end_block);
        need_end = true;
    }

    false_block->insertInto(parent);
    GetBuilder().SetInsertPoint(false_block);
    if (statement.False)
    {
        Gen(statement.False);
        false_block = GetBuilder().GetInsertBlock();
        if (!false_block->getTerminator())
        {
            GetBuilder().CreateBr(end_block);
            need_end = true;
        }
    }
    else need_end = false;

    if (need_end)
    {
        end_block->insertInto(parent);
        GetBuilder().SetInsertPoint(end_block);
    }
}

void csaw::Builder::Gen(const RetStatement& statement)
{
    if (!statement.Value)
    {
        GetBuilder().CreateRetVoid();
        return;
    }

    const auto type = m_Signatures[GetBuilder().GetInsertBlock()->getParent()].Result;

    const auto value = Gen(statement.Value, type);
    if (!value)
        return;

    const auto cast = Cast(value, type);
    if (!cast)
        return ThrowErrorStmt(statement, false, "Failed to cast: %s", cast.Msg().c_str());

    GetBuilder().CreateRet(cast.Get()->GetValue());
}

void csaw::Builder::Gen(const ScopeStatement& statement)
{
    PushScopeStack();
    for (const auto& ptr : statement.Body)
        Gen(ptr);
    PopScopeStack();
}

void csaw::Builder::Gen(const WhileStatement& statement)
{
    const auto parent = GetBuilder().GetInsertBlock()->getParent();
    const auto bkp_block = GetBuilder().GetInsertBlock();

    const auto hdr_block = llvm::BasicBlock::Create(GetContext(), "hdr", parent);
    const auto loop_block = llvm::BasicBlock::Create(GetContext(), "loop");
    const auto end_block = llvm::BasicBlock::Create(GetContext(), "end");

    const auto br_inst = GetBuilder().CreateBr(hdr_block);

    GetBuilder().SetInsertPoint(hdr_block);
    const auto condition = Gen(statement.Condition, Type::GetInt1());
    if (!condition)
    {
        br_inst->eraseFromParent();
        hdr_block->eraseFromParent();
        GetBuilder().SetInsertPoint(bkp_block);
        return;
    }
    GetBuilder().CreateCondBr(condition->GetBoolValue(), loop_block, end_block);

    loop_block->insertInto(parent);
    GetBuilder().SetInsertPoint(loop_block);
    if (statement.Body) Gen(statement.Body);
    GetBuilder().CreateBr(hdr_block);

    end_block->insertInto(parent);
    GetBuilder().SetInsertPoint(end_block);
}
