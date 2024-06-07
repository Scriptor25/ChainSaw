#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>

csaw::RValuePtr csaw::Builder::Gen(const CastExpression& expression)
{
    const auto value = Gen(expression.Value, expression.Type);
    if (!value)
        return nullptr;

    const auto cast = Cast(value, expression.Type);
    if (AssertStmt(cast, expression, false, "Failed to cast: %s", cast.Msg().c_str()))
        return nullptr;

    return cast.Get();
}
