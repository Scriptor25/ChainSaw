#include <csaw/Error.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Value.hpp>
#include <csaw/lang/Expr.hpp>

csaw::ValuePtr csaw::Builder::Gen(const SelectExpression& expression)
{
    const auto condition = Gen(expression.Condition);
    if (!condition)
        return nullptr;

    const auto parent = m_Builder->GetInsertBlock()->getParent();
    const auto bkp_block = m_Builder->GetInsertBlock();

    auto true_block = llvm::BasicBlock::Create(*m_Context, "true", parent);
    auto false_block = llvm::BasicBlock::Create(*m_Context, "false");
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end");

    const auto br_inst = m_Builder->CreateCondBr(condition->GetBoolValue(this), true_block, false_block);

    m_Builder->SetInsertPoint(true_block);
    const auto true_value = Gen(expression.True);
    if (!true_value)
    {
        br_inst->eraseFromParent();
        true_block->eraseFromParent();
        m_Builder->SetInsertPoint(bkp_block);
        return nullptr;
    }

    auto true_rvalue = true_value->GetRValue();
    true_block = m_Builder->GetInsertBlock();

    false_block->insertInto(parent);
    m_Builder->SetInsertPoint(false_block);
    const auto false_value = Gen(expression.False);
    if (!false_value)
    {
        true_block->eraseFromParent();
        false_block->eraseFromParent();
        m_Builder->SetInsertPoint(bkp_block);
        return nullptr;
    }
    auto false_rvalue = false_value->GetRValue();
    false_block = m_Builder->GetInsertBlock();

    if (false_rvalue->GetType()->ParentOf(true_rvalue->GetType()))
    {
        m_Builder->SetInsertPoint(true_block);
        const auto cast = Cast(true_rvalue, false_rvalue->GetType());
        if (!cast)
        {
            true_block->eraseFromParent();
            false_block->eraseFromParent();
            m_Builder->SetInsertPoint(bkp_block);
            ThrowErrorStmt(expression, false, "Failed to cast: %s", cast.Msg().c_str());
            return nullptr;
        }
        true_rvalue = cast.Get();
    }
    else
    {
        m_Builder->SetInsertPoint(false_block);
        const auto cast = Cast(false_rvalue, true_rvalue->GetType());
        if (!cast)
        {
            true_block->eraseFromParent();
            false_block->eraseFromParent();
            m_Builder->SetInsertPoint(bkp_block);
            ThrowErrorStmt(expression, false, "Failed to cast: %s", cast.Msg().c_str());
            return nullptr;
        }
        false_rvalue = cast.Get();
    }

    m_Builder->SetInsertPoint(true_block);
    m_Builder->CreateBr(end_block);

    m_Builder->SetInsertPoint(false_block);
    m_Builder->CreateBr(end_block);

    end_block->insertInto(parent);
    m_Builder->SetInsertPoint(end_block);

    if (true_value->IsLValue() && false_value->IsLValue())
    {
        if (true_rvalue->GetType() != false_rvalue->GetType())
        {
            true_block->eraseFromParent();
            false_block->eraseFromParent();
            end_block->eraseFromParent();
            m_Builder->SetInsertPoint(bkp_block);
            ThrowErrorStmt(expression, false, "Select results must not differ in type: %s and %s", true_rvalue->GetType()->Name.c_str(), false_rvalue->GetType()->Name.c_str());
            return nullptr;
        }

        const auto true_lvalue = std::dynamic_pointer_cast<LValue>(true_value);
        const auto false_lvalue = std::dynamic_pointer_cast<LValue>(false_value);

        const auto phi = m_Builder->CreatePHI(m_Builder->getPtrTy(), 2);
        phi->addIncoming(true_lvalue->GetPointer(), true_block);
        phi->addIncoming(false_lvalue->GetPointer(), false_block);
        return LValue::Direct(this, true_lvalue->GetType(), phi);
    }

    const auto type = Gen(true_rvalue->GetType());
    if (!type)
    {
        true_block->eraseFromParent();
        false_block->eraseFromParent();
        end_block->eraseFromParent();
        m_Builder->SetInsertPoint(bkp_block);
        ThrowErrorStmt(expression, false, "Failed to generate type %s: %s", true_rvalue->GetType()->Name.c_str(), type.Msg().c_str());
        return nullptr;
    }

    const auto phi = m_Builder->CreatePHI(type.Get(), 2);
    phi->addIncoming(true_rvalue->GetValue(), true_block);
    phi->addIncoming(false_rvalue->GetValue(), false_block);
    return RValue::Create(true_rvalue->GetType(), phi);
}
