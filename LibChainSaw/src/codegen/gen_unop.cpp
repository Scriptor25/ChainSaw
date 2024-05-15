#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Value.hpp>

csaw::RValuePtr csaw::Builder::GenNeg(const RValuePtr& value) const
{
    const auto vty = Gen(value->GetType());

    if (vty->isIntegerTy())
    {
        const auto result = m_Builder->CreateNeg(value->GetValue());
        return RValue::Direct(value->GetType(), result);
    }

    if (vty->isFloatingPointTy())
    {
        const auto result = m_Builder->CreateFNeg(value->GetValue());
        return RValue::Direct(value->GetType(), result);
    }

    return {};
}

csaw::RValuePtr csaw::Builder::GenNot(const RValuePtr& value) const
{
    const auto vty = Gen(value->GetType());

    if (vty->isIntegerTy(1))
    {
        const auto result = m_Builder->CreateNot(value->GetValue());
        return RValue::Direct(value->GetType(), result);
    }

    return {};
}

csaw::RValuePtr csaw::Builder::GenInv(const RValuePtr& value) const
{
    return {};
}

csaw::RValuePtr csaw::Builder::GenInc(const RValuePtr& value) const
{
    const auto vty = Gen(value->GetType());

    if (vty->isIntegerTy())
    {
        const auto result = m_Builder->CreateAdd(value->GetValue(), llvm::ConstantInt::get(vty, 1, true));
        return RValue::Direct(value->GetType(), result);
    }

    return {};
}

csaw::RValuePtr csaw::Builder::GenDec(const RValuePtr& value) const
{
    const auto vty = Gen(value->GetType());

    if (vty->isIntegerTy())
    {
        const auto result = m_Builder->CreateSub(value->GetValue(), llvm::ConstantInt::get(vty, 1, true));
        return RValue::Direct(value->GetType(), result);
    }

    return {};
}
