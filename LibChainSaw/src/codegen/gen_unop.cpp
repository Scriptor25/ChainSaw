#include <csaw/Builder.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::ValuePtr csaw::Builder::GenNeg(const ValuePtr& value)
{
    const auto vty = Gen(value->GetType()).Get();

    if (vty->isIntegerTy())
    {
        const auto result = GetBuilder().CreateNeg(value->GetValue());
        return RValue::Create(this, value->GetType(), result);
    }

    if (vty->isFloatingPointTy())
    {
        const auto result = GetBuilder().CreateFNeg(value->GetValue());
        return RValue::Create(this, value->GetType(), result);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenNot(const ValuePtr& value)
{
    const auto result = GetBuilder().CreateNot(value->GetBoolValue());
    return RValue::Create(this, value->GetType(), result);
}

csaw::ValuePtr csaw::Builder::GenInv(const ValuePtr& value)
{
    const auto vty = Gen(value->GetType()).Get();

    if (vty->isIntegerTy())
    {
        const auto result = GetBuilder().CreateNot(value->GetValue());
        return RValue::Create(this, value->GetType(), result);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenInc(const ValuePtr& value)
{
    const auto vty = Gen(value->GetType()).Get();

    if (vty->isIntegerTy())
    {
        const auto result = GetBuilder().CreateAdd(value->GetValue(), llvm::ConstantInt::get(vty, 1, true));
        return RValue::Create(this, value->GetType(), result);
    }

    if (vty->isFloatingPointTy())
    {
        const auto result = GetBuilder().CreateAdd(value->GetValue(), llvm::ConstantFP::get(vty, 1));
        return RValue::Create(this, value->GetType(), result);
    }

    if (vty->isPointerTy())
    {
        const auto result = GetBuilder().CreateGEP(Gen(value->GetType()->AsPointer()->Base).Get(), value->GetValue(), {llvm::ConstantInt::get(GetBuilder().getInt64Ty(), 1, true)});
        return RValue::Create(this, value->GetType(), result);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenDec(const ValuePtr& value)
{
    const auto vty = Gen(value->GetType()).Get();

    if (vty->isIntegerTy())
    {
        const auto result = GetBuilder().CreateSub(value->GetValue(), llvm::ConstantInt::get(vty, 1, true));
        return RValue::Create(this, value->GetType(), result);
    }

    if (vty->isFloatingPointTy())
    {
        const auto result = GetBuilder().CreateSub(value->GetValue(), llvm::ConstantFP::get(vty, 1));
        return RValue::Create(this, value->GetType(), result);
    }

    if (vty->isPointerTy())
    {
        const auto result = GetBuilder().CreateGEP(Gen(value->GetType()->AsPointer()->Base).Get(), value->GetValue(), {llvm::ConstantInt::get(GetBuilder().getInt64Ty(), -1, true)});
        return RValue::Create(this, value->GetType(), result);
    }

    return nullptr;
}
