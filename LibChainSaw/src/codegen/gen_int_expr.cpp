#include <csaw/Builder.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::ValuePtr csaw::Builder::Gen(const IntExpression& expression, const TypePtr& expected)
{
    if (expected)
    {
        if (expected == Type::GetInt1())
        {
            const auto value = GetBuilder().getInt1(expression.Value);
            return RValue::Create(this, expected, value);
        }
        if (expected == Type::GetInt8())
        {
            const auto value = GetBuilder().getInt8(expression.Value);
            return RValue::Create(this, expected, value);
        }
        if (expected == Type::GetInt16())
        {
            const auto value = GetBuilder().getInt16(expression.Value);
            return RValue::Create(this, expected, value);
        }
        if (expected == Type::GetInt32())
        {
            const auto value = GetBuilder().getInt32(expression.Value);
            return RValue::Create(this, expected, value);
        }
    }

    const auto value = GetBuilder().getInt64(expression.Value);
    return RValue::Create(this, Type::GetInt64(), value);
}
