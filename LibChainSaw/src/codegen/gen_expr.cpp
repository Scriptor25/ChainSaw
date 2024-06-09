#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>

csaw::ValuePtr csaw::Builder::Gen(const ExpressionPtr& ptr, const TypePtr& expected)
{
    if (!ptr)
        return nullptr;

    if (const auto p = std::dynamic_pointer_cast<BinaryExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<CallExpression>(ptr))
        return Gen(*p, expected);
    if (const auto p = std::dynamic_pointer_cast<CastExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<CharExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<DereferenceExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<FloatExpression>(ptr))
        return Gen(*p, expected);
    if (const auto p = std::dynamic_pointer_cast<IdentifierExpression>(ptr))
        return Gen(*p, expected);
    if (const auto p = std::dynamic_pointer_cast<IndexExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<IntExpression>(ptr))
        return Gen(*p, expected);
    if (const auto p = std::dynamic_pointer_cast<MemberExpression>(ptr))
        return Gen(*p, expected);
    if (const auto p = std::dynamic_pointer_cast<ReferenceExpression>(ptr))
        return Gen(*p, expected);
    if (const auto p = std::dynamic_pointer_cast<SelectExpression>(ptr))
        return Gen(*p, expected);
    if (const auto p = std::dynamic_pointer_cast<SizeOfExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<StringExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<UnaryExpression>(ptr))
        return Gen(*p);

    ThrowErrorStmt(*ptr, false, "Code generation is not implemented for this expression");
    return nullptr;
}
