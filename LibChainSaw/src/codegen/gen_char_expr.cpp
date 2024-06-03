#include <csaw/Builder.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::Builder::Gen(const CharExpression& expression)
{
    return RValue::Create(this, Type::GetInt8(), GetBuilder().getInt8(expression.Value));
}
