#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Parser.hpp>

csaw::Expression::Expression(const size_t line)
    : Statement(line)
{
}

csaw::BinaryExpression::BinaryExpression(const size_t line, const std::string& op, const ExpressionPtr& left, const ExpressionPtr& right)
    : Expression(line), Operator(op), Left(left), Right(right)
{
}

csaw::CallExpression::CallExpression(const size_t line, const ExpressionPtr& callee, const std::vector<ExpressionPtr>& args)
    : Expression(line), Callee(callee), Args(args)
{
}

csaw::CastExpression::CastExpression(const size_t line, const TypePtr& type, const ExpressionPtr& castee)
    : Expression(line), Type(type), Castee(castee)
{
}

csaw::CharExpression::CharExpression(const size_t line, const char value)
    : Expression(line), Value(value)
{
}

csaw::CharExpression::CharExpression(const size_t line, const std::string& value)
    : Expression(line)
{
    // TODO: multichar for unicode etc.
    Value = value[0];
}

csaw::IdentifierExpression::IdentifierExpression(const size_t line, const std::string& id)
    : Expression(line), Id(id)
{
}

csaw::IndexExpression::IndexExpression(const size_t line, const ExpressionPtr& array, const ExpressionPtr& index)
    : Expression(line), Array(array), Index(index)
{
}

csaw::MemberExpression::MemberExpression(const size_t line, const ExpressionPtr& object, const std::string& member)
    : Expression(line), Object(object), Member(member)
{
}

csaw::NumberExpression::NumberExpression(const size_t line, const double value)
    : Expression(line), Value(value)
{
}

csaw::NumberExpression::NumberExpression(const size_t line, const std::string& value, const int base)
    : Expression(line)
{
    switch (base)
    {
    case 2:
    case 16: Value = static_cast<double>(std::stoll(value, nullptr, base));
        break;

    case 10: Value = std::stod(value);
        break;

    default: throw std::runtime_error("unhandled base");
    }
}

csaw::SelectExpression::SelectExpression(const size_t line, const ExpressionPtr& condition, const ExpressionPtr& _true, const ExpressionPtr& _false)
    : Expression(line), Condition(condition), True(_true), False(_false)
{
}

csaw::StringExpression::StringExpression(const size_t line, const std::string& value)
    : Expression(line), Value(value)
{
}

csaw::UnaryExpression::UnaryExpression(const size_t line, const std::string& op, const ExpressionPtr& value, const bool rightop)
    : Expression(line), Operator(op), Value(value), RightOp(rightop)
{
}

csaw::VarArgExpression::VarArgExpression(const size_t line, const TypePtr& type)
    : Expression(line), Type(type)
{
}
