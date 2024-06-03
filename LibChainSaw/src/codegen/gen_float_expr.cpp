#include <csaw/Builder.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::Builder::Gen(const FloatExpression& expression, const TypePtr& expected)
{
    if (expected)
    {
        if (expected == Type::GetFlt16())
        {
            const auto value = llvm::ConstantFP::get(GetBuilder().getHalfTy(), expression.Value);
            return RValue::Create(this, expected, value);
        }
        if (expected == Type::GetFlt32())
        {
            const auto value = llvm::ConstantFP::get(GetBuilder().getFloatTy(), expression.Value);
            return RValue::Create(this, expected, value);
        }
    }

    const auto value = llvm::ConstantFP::get(GetBuilder().getDoubleTy(), expression.Value);
    return RValue::Create(this, Type::GetFlt64(), value);
}
