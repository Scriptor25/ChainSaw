#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::ValuePtr csaw::Builder::Gen(const SelectExpression& expression, const TypePtr& expected)
{
    const auto condition = Gen(expression.Condition, Type::GetInt1());
    if (!condition)
        return nullptr;

    const auto parent = GetBuilder().GetInsertBlock()->getParent();
    const auto bkp_block = GetBuilder().GetInsertBlock();

    auto true_block = llvm::BasicBlock::Create(GetContext(), "true", parent);
    auto false_block = llvm::BasicBlock::Create(GetContext(), "false");
    const auto end_block = llvm::BasicBlock::Create(GetContext(), "end");

    const auto br_inst = GetBuilder().CreateCondBr(condition->GetBoolValue(), true_block, false_block);

    GetBuilder().SetInsertPoint(true_block);
    const auto true_value = Gen(expression.True, expected);
    if (!true_value)
    {
        br_inst->eraseFromParent();
        true_block->eraseFromParent();
        GetBuilder().SetInsertPoint(bkp_block);
        return nullptr;
    }

    auto true_rvalue = true_value->GetRValue();
    true_block = GetBuilder().GetInsertBlock();

    false_block->insertInto(parent);
    GetBuilder().SetInsertPoint(false_block);
    const auto false_value = Gen(expression.False, expected);
    if (!false_value)
    {
        br_inst->eraseFromParent();
        true_block->eraseFromParent();
        false_block->eraseFromParent();
        GetBuilder().SetInsertPoint(bkp_block);
        return nullptr;
    }
    auto false_rvalue = false_value->GetRValue();
    false_block = GetBuilder().GetInsertBlock();

    const auto both_lvalue = true_value->IsLValue() && false_value->IsLValue();

    if (!both_lvalue && false_rvalue->GetType() != true_rvalue->GetType())
    {
        if (false_rvalue->GetType()->ParentOf(true_rvalue->GetType()))
        {
            GetBuilder().SetInsertPoint(true_block);
            const auto cast = Cast(true_rvalue, false_rvalue->GetType());
            if (!cast)
            {
                br_inst->eraseFromParent();
                true_block->eraseFromParent();
                false_block->eraseFromParent();
                GetBuilder().SetInsertPoint(bkp_block);
                ThrowErrorStmt(expression, false, "Failed to cast: %s", cast.Msg().c_str());
                return nullptr;
            }
            true_rvalue = cast.Get()->GetRValue();
        }
        else if (true_rvalue->GetType()->ParentOf(false_rvalue->GetType()))
        {
            GetBuilder().SetInsertPoint(false_block);
            const auto cast = Cast(false_rvalue, true_rvalue->GetType());
            if (!cast)
            {
                br_inst->eraseFromParent();
                true_block->eraseFromParent();
                false_block->eraseFromParent();
                GetBuilder().SetInsertPoint(bkp_block);
                ThrowErrorStmt(expression, false, "Failed to cast: %s", cast.Msg().c_str());
                return nullptr;
            }
            false_rvalue = cast.Get()->GetRValue();
        }
        else
        {
            br_inst->eraseFromParent();
            true_block->eraseFromParent();
            false_block->eraseFromParent();
            GetBuilder().SetInsertPoint(bkp_block);
            ThrowErrorStmt(expression, false, "Select results must not differ in type: %s and %s", true_rvalue->GetType()->Name.c_str(), false_rvalue->GetType()->Name.c_str());
            return nullptr;
        }
    }

    GetBuilder().SetInsertPoint(true_block);
    GetBuilder().CreateBr(end_block);

    GetBuilder().SetInsertPoint(false_block);
    GetBuilder().CreateBr(end_block);

    end_block->insertInto(parent);
    GetBuilder().SetInsertPoint(end_block);

    if (both_lvalue)
    {
        if (true_rvalue->GetType() != false_rvalue->GetType())
        {
            br_inst->eraseFromParent();
            true_block->eraseFromParent();
            false_block->eraseFromParent();
            end_block->eraseFromParent();
            GetBuilder().SetInsertPoint(bkp_block);
            ThrowErrorStmt(expression, false, "Select results must not differ in type: %s and %s", true_rvalue->GetType()->Name.c_str(), false_rvalue->GetType()->Name.c_str());
            return nullptr;
        }

        const auto true_lvalue = std::dynamic_pointer_cast<LValue>(true_value);
        const auto false_lvalue = std::dynamic_pointer_cast<LValue>(false_value);

        const auto phi = GetBuilder().CreatePHI(GetBuilder().getPtrTy(), 2);
        phi->addIncoming(true_lvalue->GetPointer(), true_block);
        phi->addIncoming(false_lvalue->GetPointer(), false_block);
        return LValue::Direct(this, true_lvalue->GetType(), phi);
    }

    const auto type = Gen(true_rvalue->GetType());
    if (!type)
    {
        br_inst->eraseFromParent();
        true_block->eraseFromParent();
        false_block->eraseFromParent();
        end_block->eraseFromParent();
        GetBuilder().SetInsertPoint(bkp_block);
        ThrowErrorStmt(expression, false, "Failed to generate type %s: %s", true_rvalue->GetType()->Name.c_str(), type.Msg().c_str());
        return nullptr;
    }

    const auto phi = GetBuilder().CreatePHI(type.Get(), 2);
    phi->addIncoming(true_rvalue->GetValue(), true_block);
    phi->addIncoming(false_rvalue->GetValue(), false_block);
    return RValue::Create(this, true_rvalue->GetType(), phi);
}
