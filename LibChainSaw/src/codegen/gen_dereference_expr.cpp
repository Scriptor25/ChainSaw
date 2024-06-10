#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Value.hpp>

csaw::ValuePtr csaw::Builder::Gen(const DereferenceExpression& expression)
{
    const auto value = Gen(expression.Value, nullptr);
    if (!value)
        return nullptr;

    const auto deref = value->Dereference();
    if (AssertStmt(deref, expression, false, "Failed to dereference: %s", deref.Msg().c_str()))
        return nullptr;

    return deref.Get();
}
