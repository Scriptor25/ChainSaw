#include <utility>
#include <csaw/Builder.hpp>
#include <csaw/Expect.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::Expect<csaw::ValuePtr> csaw::Builder::Cast(const ValuePtr& value, const TypePtr& type)
{
    if (!value)
        return Expect<ValuePtr>("Value is empty");

    if (!type)
        return Expect<ValuePtr>("Type is empty");

    if (value->GetType() == type)
        return value;

    const auto tty = Gen(type);
    if (!tty)
        return Expect<ValuePtr>("Type is null: " + tty.Msg());

    const auto vty = Gen(value->GetType());
    if (!vty)
        return Expect<ValuePtr>("Value type is null: " + vty.Msg());

    if (vty.Get()->isPointerTy())
    {
        if (tty.Get()->isPointerTy())
        {
            const auto result = GetBuilder().CreatePointerCast(value->GetValue(), tty.Get());
            return {RValue::Create(this, type, result)};
        }

        if (tty.Get()->isIntegerTy())
        {
            const auto result = GetBuilder().CreatePtrToInt(value->GetValue(), tty.Get());
            return {RValue::Create(this, type, result)};
        }
    }

    if (vty.Get()->isIntegerTy())
    {
        if (tty.Get()->isPointerTy())
        {
            const auto result = GetBuilder().CreateIntToPtr(value->GetValue(), tty.Get());
            return {RValue::Create(this, type, result)};
        }

        if (tty.Get()->isIntegerTy())
        {
            const auto result = GetBuilder().CreateIntCast(value->GetValue(), tty.Get(), true);
            return {RValue::Create(this, type, result)};
        }

        if (tty.Get()->isFloatingPointTy())
        {
            const auto result = GetBuilder().CreateSIToFP(value->GetValue(), tty.Get());
            return {RValue::Create(this, type, result)};
        }
    }

    if (vty.Get()->isFloatingPointTy())
    {
        if (tty.Get()->isIntegerTy())
        {
            const auto result = GetBuilder().CreateFPToSI(value->GetValue(), tty.Get());
            return {RValue::Create(this, type, result)};
        }

        if (tty.Get()->isFloatingPointTy())
        {
            const auto result = GetBuilder().CreateFPCast(value->GetValue(), tty.Get());
            return {RValue::Create(this, type, result)};
        }
    }

    return Expect<ValuePtr>("Cast from " + value->GetType()->Name + " to " + type->Name + " is not implemented");
}

csaw::Expect<csaw::RValuePair> csaw::Builder::CastToBestOf(const RValuePtr& left, const RValuePtr& right)
{
    if (!left)
        return Expect<RValuePair>("Left value is empty");

    if (!right)
        return Expect<RValuePair>("Right value is empty");

    if (left->GetType() == right->GetType())
        return {{left, right}};

    const auto lty = Gen(left->GetType());
    if (!lty)
        return Expect<RValuePair>("Left type is null: " + lty.Msg());

    const auto rty = Gen(right->GetType());
    if (!rty)
        return Expect<RValuePair>("Right type is null: " + rty.Msg());

    if (lty.Get() == rty.Get())
        return {{left, right}};

    if (lty.Get()->isPointerTy())
    {
        if (rty.Get()->isIntegerTy())
        {
            const auto value = GetBuilder().CreatePtrToInt(left->GetValue(), rty.Get());
            return {{RValue::Create(this, right->GetType(), value), right}};
        }
    }

    if (lty.Get()->isIntegerTy())
    {
        if (rty.Get()->isPointerTy())
        {
            const auto value = GetBuilder().CreatePtrToInt(right->GetValue(), lty.Get());
            return {{left, RValue::Create(this, left->GetType(), value)}};
        }

        if (rty.Get()->isIntegerTy())
        {
            if (lty.Get()->getIntegerBitWidth() > rty.Get()->getIntegerBitWidth())
            {
                const auto value = GetBuilder().CreateIntCast(right->GetValue(), lty.Get(), true);
                return {{left, RValue::Create(this, left->GetType(), value)}};
            }

            const auto value = GetBuilder().CreateIntCast(left->GetValue(), rty.Get(), true);
            return {{RValue::Create(this, right->GetType(), value), right}};
        }

        if (rty.Get()->isFloatingPointTy())
        {
            const auto value = GetBuilder().CreateSIToFP(left->GetValue(), rty.Get());
            return {{RValue::Create(this, right->GetType(), value), right}};
        }
    }

    if (lty.Get()->isFloatingPointTy())
    {
        if (rty.Get()->isIntegerTy())
        {
            const auto value = GetBuilder().CreateSIToFP(right->GetValue(), lty.Get());
            return {{left, RValue::Create(this, left->GetType(), value)}};
        }

        if (rty.Get()->isFloatingPointTy())
        {
            if ((lty.Get()->isHalfTy() && (rty.Get()->isFloatTy() || rty.Get()->isDoubleTy()))
                || lty.Get()->isFloatTy() && rty.Get()->isDoubleTy())
            {
                const auto value = GetBuilder().CreateFPCast(left->GetValue(), rty.Get());
                return {{RValue::Create(this, right->GetType(), value), right}};
            }

            const auto value = GetBuilder().CreateFPCast(right->GetValue(), lty.Get());
            return {{left, RValue::Create(this, left->GetType(), value)}};
        }
    }

    return Expect<RValuePair>("Implicit casting between " + left->GetType()->Name + " and " + right->GetType()->Name + " is not implemented");
}
