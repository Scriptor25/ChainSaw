#include <utility>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Expect.hpp>
#include <csaw/codegen/Value.hpp>

csaw::Expect<csaw::RValuePtr> csaw::Builder::Cast(const ValuePtr& value, const TypePtr& type) const
{
    if (!value)
        return Expect<RValuePtr>("Value is empty");

    if (!type)
        return Expect<RValuePtr>("Type is empty");

    if (value->GetType() == type)
        return value->GetRValue();

    const auto tty = Gen(type);
    if (!tty)
        return Expect<RValuePtr>("Type is null: " + tty.Msg());

    const auto vty = Gen(value->GetType());
    if (!vty)
        return Expect<RValuePtr>("Value type is null: " + vty.Msg());

    if (vty.Get()->isPointerTy())
    {
        if (tty.Get()->isPointerTy())
        {
            const auto result = m_Builder->CreatePointerCast(value->GetValue(), tty.Get());
            return RValue::Create(type, result);
        }

        if (tty.Get()->isIntegerTy())
        {
            const auto result = m_Builder->CreatePtrToInt(value->GetValue(), tty.Get());
            return RValue::Create(type, result);
        }
    }

    if (vty.Get()->isIntegerTy())
    {
        if (tty.Get()->isPointerTy())
        {
            const auto result = m_Builder->CreateIntToPtr(value->GetValue(), tty.Get());
            return RValue::Create(type, result);
        }

        if (tty.Get()->isIntegerTy())
        {
            const auto result = m_Builder->CreateIntCast(value->GetValue(), tty.Get(), true);
            return RValue::Create(type, result);
        }

        if (tty.Get()->isFloatingPointTy())
        {
            const auto result = m_Builder->CreateSIToFP(value->GetValue(), tty.Get());
            return RValue::Create(type, result);
        }
    }

    if (vty.Get()->isFloatingPointTy())
    {
        if (tty.Get()->isIntegerTy())
        {
            const auto result = m_Builder->CreateFPToSI(value->GetValue(), tty.Get());
            return RValue::Create(type, result);
        }

        if (tty.Get()->isFloatingPointTy())
        {
            const auto result = m_Builder->CreateFPCast(value->GetValue(), tty.Get());
            return RValue::Create(type, result);
        }
    }

    return Expect<RValuePtr>("Cast from " + value->GetType()->Name + " to " + type->Name + " is not implemented");
}

csaw::Expect<csaw::RValPair> csaw::Builder::CastToBestOf(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left)
        return Expect<RValPair>("Left value is empty");

    if (!right)
        return Expect<RValPair>("Right value is empty");

    if (left->GetType() == right->GetType())
        return {{left, right}};

    const auto lty = Gen(left->GetType());
    if (!lty)
        return Expect<RValPair>("Left type is null: " + lty.Msg());
    const auto rty = Gen(right->GetType());
    if (!rty)
        return Expect<RValPair>("Right type is null: " + rty.Msg());

    if (lty.Get()->isIntegerTy())
    {
        if (rty.Get()->isIntegerTy())
        {
            if (lty.Get()->getIntegerBitWidth() > rty.Get()->getIntegerBitWidth())
            {
                const auto value = m_Builder->CreateIntCast(right->GetValue(), lty.Get(), true);
                return {{left, RValue::Create(left->GetType(), value)}};
            }

            const auto value = m_Builder->CreateIntCast(left->GetValue(), rty.Get(), true);
            return {{RValue::Create(right->GetType(), value), right}};
        }

        if (rty.Get()->isFloatingPointTy())
        {
            const auto value = m_Builder->CreateSIToFP(left->GetValue(), rty.Get());
            return {{RValue::Create(right->GetType(), value), right}};
        }
    }

    if (lty.Get()->isFloatingPointTy())
    {
        if (rty.Get()->isIntegerTy())
        {
            const auto value = m_Builder->CreateSIToFP(right->GetValue(), lty.Get());
            return {{left, RValue::Create(left->GetType(), value)}};
        }
    }

    return Expect<RValPair>("Superior cast between " + left->GetType()->Name + " and " + right->GetType()->Name + " is not implemented");
}
