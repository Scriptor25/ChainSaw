#include <utility>
#include <csaw/Builder.hpp>
#include <csaw/Expect.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

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
            const auto result = GetBuilder().CreatePointerCast(value->GetValue(), tty.Get());
            return RValue::Create(type, result);
        }

        if (tty.Get()->isIntegerTy())
        {
            const auto result = GetBuilder().CreatePtrToInt(value->GetValue(), tty.Get());
            return RValue::Create(type, result);
        }
    }

    if (vty.Get()->isArrayTy())
    {
        if (tty.Get()->isPointerTy())
        {
            const auto lvalue = std::dynamic_pointer_cast<LValue>(value);
            const auto result = GetBuilder().CreateGEP(vty.Get(), lvalue->GetPointer(), {GetBuilder().getInt64(0), GetBuilder().getInt64(0)});
            return RValue::Create(type, result);
        }
    }

    if (vty.Get()->isIntegerTy())
    {
        if (tty.Get()->isPointerTy())
        {
            const auto result = GetBuilder().CreateIntToPtr(value->GetValue(), tty.Get());
            return RValue::Create(type, result);
        }

        if (tty.Get()->isIntegerTy())
        {
            const auto result = GetBuilder().CreateIntCast(value->GetValue(), tty.Get(), true);
            return RValue::Create(type, result);
        }

        if (tty.Get()->isFloatingPointTy())
        {
            const auto result = GetBuilder().CreateSIToFP(value->GetValue(), tty.Get());
            return RValue::Create(type, result);
        }
    }

    if (vty.Get()->isFloatingPointTy())
    {
        if (tty.Get()->isIntegerTy())
        {
            const auto result = GetBuilder().CreateFPToSI(value->GetValue(), tty.Get());
            return RValue::Create(type, result);
        }

        if (tty.Get()->isFloatingPointTy())
        {
            const auto result = GetBuilder().CreateFPCast(value->GetValue(), tty.Get());
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
                const auto value = GetBuilder().CreateIntCast(right->GetValue(), lty.Get(), true);
                return {{left, RValue::Create(left->GetType(), value)}};
            }

            const auto value = GetBuilder().CreateIntCast(left->GetValue(), rty.Get(), true);
            return {{RValue::Create(right->GetType(), value), right}};
        }

        if (rty.Get()->isFloatingPointTy())
        {
            const auto value = GetBuilder().CreateSIToFP(left->GetValue(), rty.Get());
            return {{RValue::Create(right->GetType(), value), right}};
        }
    }

    if (lty.Get()->isFloatingPointTy())
    {
        if (rty.Get()->isIntegerTy())
        {
            const auto value = GetBuilder().CreateSIToFP(right->GetValue(), lty.Get());
            return {{left, RValue::Create(left->GetType(), value)}};
        }
    }

    return Expect<RValPair>("Superior cast between " + left->GetType()->Name + " and " + right->GetType()->Name + " is not implemented");
}
