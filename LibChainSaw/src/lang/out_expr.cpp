#include <ostream>
#include <csaw/Message.hpp>
#include <csaw/lang/Expr.hpp>

std::ostream& csaw::operator<<(std::ostream& out, const ExpressionPtr& ptr)
{
    if (const auto expr = std::dynamic_pointer_cast<BinaryExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<CallExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<CastExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<CharExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<DereferenceExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<FloatExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<IdentifierExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<IndexExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<IntExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<MemberExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<ReferenceExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<SelectExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<StringExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<UnaryExpression>(ptr))
        return out << *expr;
    if (const auto expr = std::dynamic_pointer_cast<VarArgExpression>(ptr))
        return out << *expr;

    CSAW_MESSAGE_STMT(true, *ptr, "Printing is not implemented for this expression");
    return out;
}

std::ostream& csaw::operator<<(std::ostream& out, const BinaryExpression& expression)
{
    return out << expression.Left << ' ' << expression.Operator << ' ' << expression.Right;
}

std::ostream& csaw::operator<<(std::ostream& out, const CallExpression& expression)
{
    out << expression.Callee << '(';
    for (size_t i = 0; i < expression.Args.size(); i++)
    {
        if (i > 0)
            out << ", ";
        out << expression.Args[i];
    }
    return out << ')';
}

std::ostream& csaw::operator<<(std::ostream& out, const CastExpression& expression)
{
    return out << '[' << expression.Type << ']' << expression.Value;
}

std::ostream& csaw::operator<<(std::ostream& out, const CharExpression& expression)
{
    return out << '\'' << expression.Value << '\'';
}

std::ostream& csaw::operator<<(std::ostream& out, const DereferenceExpression& expression)
{
    return out << '*' << expression.Value;
}

std::ostream& csaw::operator<<(std::ostream& out, const FloatExpression& expression)
{
    return out << expression.Value;
}

std::ostream& csaw::operator<<(std::ostream& out, const IdentifierExpression& expression)
{
    return out << expression.Id;
}

std::ostream& csaw::operator<<(std::ostream& out, const IndexExpression& expression)
{
    return out << expression.Array << '[' << expression.Index << ']';
}

std::ostream& csaw::operator<<(std::ostream& out, const IntExpression& expression)
{
    return out << expression.Value;
}

std::ostream& csaw::operator<<(std::ostream& out, const MemberExpression& expression)
{
    return out << expression.Object << '.' << expression.Member;
}

std::ostream& csaw::operator<<(std::ostream& out, const ReferenceExpression& expression)
{
    return out << '&' << expression.Value;
}

std::ostream& csaw::operator<<(std::ostream& out, const SelectExpression& expression)
{
    return out << expression.Condition << " ? " << expression.True << " : " << expression.False;
}

std::ostream& csaw::operator<<(std::ostream& out, const StringExpression& expression)
{
    std::string value;
    for (const auto c : expression.Value)
    {
        switch (c)
        {
        case '\a': value += "\\a";
            break;
        case '\b': value += "\\b";
            break;
        case '\f': value += "\\f";
            break;
        case '\n': value += "\\n";
            break;
        case '\r': value += "\\r";
            break;
        case '\t': value += "\\t";
            break;
        case '\v': value += "\\v";
            break;

        default: value += c;
            break;
        }
    }

    return out << '"' << value << '"';
}

std::ostream& csaw::operator<<(std::ostream& out, const UnaryExpression& expression)
{
    if (!expression.OpRight) out << expression.Operator;
    out << expression.Value;
    if (expression.OpRight) out << expression.Operator;
    return out;
}

std::ostream& csaw::operator<<(std::ostream& out, const VarArgExpression& expression)
{
    return out << "?[" << expression.Type << ']';
}
