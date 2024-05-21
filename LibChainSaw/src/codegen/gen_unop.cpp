#include <csaw/Builder.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::Builder::GenNeg(const RValuePtr& value) const
{
    const auto vty = Gen(value->GetType()).Get();

    if (vty->isIntegerTy())
    {
        const auto result = GetBuilder().CreateNeg(value->GetValue());
        return RValue::Create(value->GetType(), result);
    }

    if (vty->isFloatingPointTy())
    {
        const auto result = GetBuilder().CreateFNeg(value->GetValue());
        return RValue::Create(value->GetType(), result);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenNot(const RValuePtr& value) const
{
    const auto result = GetBuilder().CreateNot(value->GetBoolValue(this));
    return RValue::Create(value->GetType(), result);
}

csaw::RValuePtr csaw::Builder::GenInv(const RValuePtr& value) const
{
    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenInc(const RValuePtr& value) const
{
    const auto vty = Gen(value->GetType()).Get();

    if (vty->isIntegerTy())
    {
        const auto result = GetBuilder().CreateAdd(value->GetValue(), llvm::ConstantInt::get(vty, 1, true));
        return RValue::Create(value->GetType(), result);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenDec(const RValuePtr& value) const
{
    const auto vty = Gen(value->GetType()).Get();

    if (vty->isIntegerTy())
    {
        const auto result = GetBuilder().CreateSub(value->GetValue(), llvm::ConstantInt::get(vty, 1, true));
        return RValue::Create(value->GetType(), result);
    }

    return nullptr;
}
