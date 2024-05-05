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

csaw::TypePtr csaw::BinaryExpression::GetType() const
{
    throw std::runtime_error("not yet implemented");
}

csaw::CallExpression::CallExpression(const size_t line, const ExpressionPtr& callee, const std::vector<ExpressionPtr>& args)
    : Expression(line), Callee(callee), Args(args)
{
}

csaw::TypePtr csaw::CallExpression::GetType() const
{
    throw std::runtime_error("not yet implemented");
}

csaw::CastExpression::CastExpression(const size_t line, const TypePtr& type, const ExpressionPtr& value)
    : Expression(line), Type(type), Value(value)
{
}

csaw::TypePtr csaw::CastExpression::GetType() const
{
    return Type;
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

csaw::TypePtr csaw::CharExpression::GetType() const
{
    return Type::Get("int8");
}

csaw::FloatExpression::FloatExpression(const size_t line, const std::string& value)
    : Expression(line), Value(std::stod(value))
{
}

csaw::TypePtr csaw::FloatExpression::GetType() const
{
    return Type::Get("flt64");
}

csaw::IdentifierExpression::IdentifierExpression(const size_t line, const std::string& id)
    : Expression(line), Id(id)
{
}

csaw::TypePtr csaw::IdentifierExpression::GetType() const
{
    throw std::runtime_error("not yet implemented");
}

csaw::IndexExpression::IndexExpression(const size_t line, const ExpressionPtr& array, const ExpressionPtr& index)
    : Expression(line), Array(array), Index(index)
{
}

csaw::TypePtr csaw::IndexExpression::GetType() const
{
    throw std::runtime_error("not yet implemented");
}

csaw::IntExpression::IntExpression(const size_t line, const std::string& value, const int base)
    : Expression(line), Value(std::stoull(value, nullptr, base))
{
}

csaw::TypePtr csaw::IntExpression::GetType() const
{
    return Type::Get("int64");
}

csaw::MemberExpression::MemberExpression(const size_t line, const ExpressionPtr& object, const std::string& member)
    : Expression(line), Object(object), Member(member)
{
}

csaw::TypePtr csaw::MemberExpression::GetType() const
{
    throw std::runtime_error("not yet implemented");
}

csaw::SelectExpression::SelectExpression(const size_t line, const ExpressionPtr& condition, const ExpressionPtr& _true, const ExpressionPtr& _false)
    : Expression(line), Condition(condition), True(_true), False(_false)
{
}

csaw::TypePtr csaw::SelectExpression::GetType() const
{
    if (const auto type = True->GetType()) return type;
    return False->GetType();
}

csaw::StringExpression::StringExpression(const size_t line, const std::string& value)
    : Expression(line), Value(value)
{
}

csaw::TypePtr csaw::StringExpression::GetType() const
{
    return PointerType::Get(Type::Get("int8"));
}

csaw::UnaryExpression::UnaryExpression(const size_t line, const std::string& op, const ExpressionPtr& value, const bool rightop)
    : Expression(line), Operator(op), Value(value), RightOp(rightop)
{
}

csaw::TypePtr csaw::UnaryExpression::GetType() const
{
    throw std::runtime_error("not yet implemented");
}

csaw::VarArgExpression::VarArgExpression(const size_t line, const TypePtr& type)
    : Expression(line), Type(type)
{
}

csaw::TypePtr csaw::VarArgExpression::GetType() const
{
    return Type;
}
