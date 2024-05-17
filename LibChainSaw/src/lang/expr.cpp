#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Parser.hpp>

csaw::Expression::Expression(const std::string& filename, const size_t line)
    : Statement(filename, line)
{
}

csaw::BinaryExpression::BinaryExpression(const std::string& filename, const size_t line, const std::string& op, const ExpressionPtr& left, const ExpressionPtr& right)
    : Expression(filename, line), Operator(op), Left(left), Right(right)
{
}

csaw::CallExpression::CallExpression(const std::string& filename, const size_t line, const ExpressionPtr& callee, const std::vector<ExpressionPtr>& args)
    : Expression(filename, line), Callee(callee), Args(args)
{
}

csaw::CastExpression::CastExpression(const std::string& filename, const size_t line, const TypePtr& type, const ExpressionPtr& value)
    : Expression(filename, line), Type(type), Value(value)
{
}

csaw::CharExpression::CharExpression(const std::string& filename, const size_t line, const char value)
    : Expression(filename, line), Value(value)
{
}

csaw::CharExpression::CharExpression(const std::string& filename, const size_t line, const std::string& value)
    : Expression(filename, line)
{
    // TODO: multichar for unicode etc.
    Value = value[0];
}

csaw::DereferenceExpression::DereferenceExpression(const std::string& filename, const size_t line, const ExpressionPtr& value)
    : Expression(filename, line), Value(value)
{
}

csaw::FloatExpression::FloatExpression(const std::string& filename, const size_t line, const std::string& value)
    : Expression(filename, line), Value(std::stod(value))
{
}

csaw::IdentifierExpression::IdentifierExpression(const std::string& filename, const size_t line, const std::string& id)
    : Expression(filename, line), Id(id)
{
}

csaw::IndexExpression::IndexExpression(const std::string& filename, const size_t line, const ExpressionPtr& array, const ExpressionPtr& index)
    : Expression(filename, line), Array(array), Index(index)
{
}

csaw::IntExpression::IntExpression(const std::string& filename, const size_t line, const uint32_t value)
    : Expression(filename, line), Value(value)
{
}

csaw::MemberExpression::MemberExpression(const std::string& filename, const size_t line, const ExpressionPtr& object, const std::string& member, const bool deref)
    : Expression(filename, line), Object(object), Member(member), ShouldDeref(deref)
{
}

csaw::ReferenceExpression::ReferenceExpression(const std::string& filename, const size_t line, const ExpressionPtr& value)
    : Expression(filename, line), Value(value)
{
}

csaw::SelectExpression::SelectExpression(const std::string& filename, const size_t line, const ExpressionPtr& condition, const ExpressionPtr& _true, const ExpressionPtr& _false)
    : Expression(filename, line), Condition(condition), True(_true), False(_false)
{
}

csaw::SizeOfExpression::SizeOfExpression(const std::string& filename, const size_t line, const TypePtr& type)
    : Expression(filename, line), Type(type)
{
}

csaw::StringExpression::StringExpression(const std::string& filename, const size_t line, const std::string& value)
    : Expression(filename, line), Value(value)
{
}

csaw::UnaryExpression::UnaryExpression(const std::string& filename, const size_t line, const std::string& op, const ExpressionPtr& value, const bool rightop)
    : Expression(filename, line), Operator(op), Value(value), OpRight(rightop)
{
}

csaw::VarArgExpression::VarArgExpression(const std::string& filename, const size_t line, const TypePtr& type)
    : Expression(filename, line), Type(type)
{
}
