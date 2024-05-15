#include <utility>
#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Value.hpp>

std::pair<csaw::RValuePtr, csaw::RValuePtr> csaw::Builder::CastToBestOf(const RValuePtr& left, const RValuePtr& right) const
{
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
                return {left, RValue::Direct(left->GetType(), value)};
            }

            const auto value = m_Builder->CreateIntCast(left->GetValue(), rty, true);
            return {RValue::Direct(right->GetType(), value), right};
        }

        if (rty->isFloatingPointTy())
        {
            const auto value = m_Builder->CreateSIToFP(left->GetValue(), rty);
            return {RValue::Direct(right->GetType(), value), right};
        }
    }

    if (lty->isFloatingPointTy())
    {
        if (rty->isIntegerTy())
        {
            const auto value = m_Builder->CreateSIToFP(right->GetValue(), lty);
            return {left, RValue::Direct(left->GetType(), value)};
        }
    }

    CSAW_MESSAGE_NONE(true, "superior cast between " + left->GetType()->Name + " and " + right->GetType()->Name + " is not implemented");
}
