#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::Builder::Gen(const ReferenceExpression& expression)
{
    const auto value = Gen(expression.Value, nullptr);
    if (!value)
        return nullptr;

    if (AssertStmt(value->IsLValue(), expression, false, "Cannot get reference to rvalue"))
        return nullptr;

    return std::dynamic_pointer_cast<LValue>(value)->GetReference();
}
