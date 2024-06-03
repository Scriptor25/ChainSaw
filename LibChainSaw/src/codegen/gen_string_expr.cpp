#include <csaw/Builder.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::Builder::Gen(const StringExpression& expression)
{
    return RValue::Create(this, PointerType::Get(Type::GetInt8()), GetBuilder().CreateGlobalStringPtr(expression.Value));
}
