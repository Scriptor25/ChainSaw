#include <utility>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Value.hpp>

csaw::RValuePtr csaw::Builder::Cast(const ValuePtr& value, const TypePtr& type) const
{
    if (!value || !type)
        return nullptr;

    if (value->GetType() == type)
        return value->GetRValue();

    const auto tty = Gen(type);
    const auto vty = Gen(value->GetType());
    if (!tty || !vty) return nullptr;

    if (vty->isPointerTy())
    {
        if (tty->isPointerTy())
        {
            const auto result = m_Builder->CreatePointerCast(value->GetValue(), tty);
            return RValue::Create(type, result);
        }

        if (tty->isIntegerTy())
        {
            const auto result = m_Builder->CreatePtrToInt(value->GetValue(), tty);
            return RValue::Create(type, result);
        }
    }

    if (vty->isIntegerTy())
    {
        if (tty->isPointerTy())
        {
            const auto result = m_Builder->CreateIntToPtr(value->GetValue(), tty);
            return RValue::Create(type, result);
        }

        if (tty->isIntegerTy())
        {
            const auto result = m_Builder->CreateIntCast(value->GetValue(), tty, true);
            return RValue::Create(type, result);
        }

        if (tty->isFloatingPointTy())
        {
            const auto result = m_Builder->CreateSIToFP(value->GetValue(), tty);
            return RValue::Create(type, result);
        }
    }

    if (vty->isFloatingPointTy())
    {
        if (tty->isIntegerTy())
        {
            const auto result = m_Builder->CreateFPToSI(value->GetValue(), tty);
            return RValue::Create(type, result);
        }

        if (tty->isFloatingPointTy())
        {
            const auto result = m_Builder->CreateFPCast(value->GetValue(), tty);
            return RValue::Create(type, result);
        }
    }

    return nullptr;
}

std::pair<csaw::RValuePtr, csaw::RValuePtr> csaw::Builder::CastToBestOf(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return {};

    if (left->GetType() == right->GetType())
        return {left, right};

    const auto lty = Gen(left->GetType());
    const auto rty = Gen(right->GetType());

    if (lty->isIntegerTy())
    {
        if (rty->isIntegerTy())
        {
            if (lty->getIntegerBitWidth() > rty->getIntegerBitWidth())
            {
                const auto value = m_Builder->CreateIntCast(right->GetValue(), lty, true);
                return {left, RValue::Create(left->GetType(), value)};
            }

            const auto value = m_Builder->CreateIntCast(left->GetValue(), rty, true);
            return {RValue::Create(right->GetType(), value), right};
        }

        if (rty->isFloatingPointTy())
        {
            const auto value = m_Builder->CreateSIToFP(left->GetValue(), rty);
            return {RValue::Create(right->GetType(), value), right};
        }
    }

    if (lty->isFloatingPointTy())
    {
        if (rty->isIntegerTy())
        {
            const auto value = m_Builder->CreateSIToFP(right->GetValue(), lty);
            return {left, RValue::Create(left->GetType(), value)};
        }
    }

    return {};
}
