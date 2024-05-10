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

csaw::CastExpression::CastExpression(const size_t line, const TypePtr& type, const ExpressionPtr& value)
    : Expression(line), Type(type), Value(value)
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

csaw::DereferenceExpression::DereferenceExpression(const size_t line, const ExpressionPtr& value)
    : Expression(line), Value(value)
{
}

csaw::FloatExpression::FloatExpression(const size_t line, const std::string& value)
    : Expression(line), Value(std::stod(value))
{
}

csaw::IdentifierExpression::IdentifierExpression(const size_t line, const std::string& id)
    : Expression(line), Id(id)
{
}

csaw::IndexExpression::IndexExpression(const size_t line, const ExpressionPtr& array, const ExpressionPtr& index)
    : Expression(line), Array(array), Index(index)
{
}

csaw::IntExpression::IntExpression(const size_t line, const uint32_t value)
    : Expression(line), Value(value)
{
}

csaw::MemberExpression::MemberExpression(const size_t line, const ExpressionPtr& object, const std::string& member, const bool deref)
    : Expression(line), Object(object), Member(member), ShouldDeref(deref)
{
}

csaw::ReferenceExpression::ReferenceExpression(const size_t line, const ExpressionPtr& value)
    : Expression(line), Value(value)
{
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
    : Expression(line), Operator(op), Value(value), OpRight(rightop)
{
}

csaw::VarArgExpression::VarArgExpression(const size_t line, const TypePtr& type)
    : Expression(line), Type(type)
{
}
