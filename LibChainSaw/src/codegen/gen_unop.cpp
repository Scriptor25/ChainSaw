#include <csaw/Builder.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::Builder::GenNeg(const RValuePtr& value)
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

csaw::RValuePtr csaw::Builder::GenNot(const RValuePtr& value)
{
    const auto result = GetBuilder().CreateNot(value->GetBoolValue());
    return RValue::Create(this, value->GetType(), result);
}

csaw::RValuePtr csaw::Builder::GenInv(const RValuePtr& value)
{
    const auto vty = Gen(value->GetType()).Get();

    if (vty->isIntegerTy())
    {
        const auto result = GetBuilder().CreateNot(value->GetValue());
        return RValue::Create(this, value->GetType(), result);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenInc(const RValuePtr& value)
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

csaw::RValuePtr csaw::Builder::GenDec(const RValuePtr& value)
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
