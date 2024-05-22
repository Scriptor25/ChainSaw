#include <string>
#include <csaw/Expr.hpp>
#include <csaw/Parser.hpp>

csaw::Expression::Expression(const SourceLoc& loc)
    : Statement(loc)
{
}

csaw::BinaryExpression::BinaryExpression(const SourceLoc& loc, const std::string& op, const ExpressionPtr& left, const ExpressionPtr& right)
    : Expression(loc), Operator(op), Left(left), Right(right)
{
}

csaw::CallExpression::CallExpression(const SourceLoc& loc, const ExpressionPtr& callee, const std::vector<TypePtr>& temp_args, const std::vector<ExpressionPtr>& args)
    : Expression(loc), Callee(callee), TempArgs(temp_args), Args(args)
{
}

csaw::CastExpression::CastExpression(const SourceLoc& loc, const TypePtr& type, const ExpressionPtr& value)
    : Expression(loc), Type(type), Value(value)
{
}

csaw::CharExpression::CharExpression(const SourceLoc& loc, const char value)
    : Expression(loc), Value(value)
{
}

csaw::CharExpression::CharExpression(const SourceLoc& loc, const std::string& value)
    : Expression(loc), Value(value[0])
{
}

csaw::DereferenceExpression::DereferenceExpression(const SourceLoc& loc, const ExpressionPtr& value)
    : Expression(loc), Value(value)
{
}

csaw::FloatExpression::FloatExpression(const SourceLoc& loc, const std::string& value)
    : Expression(loc), Value(std::stod(value))
{
}

csaw::IdentifierExpression::IdentifierExpression(const SourceLoc& loc, const std::string& id)
    : Expression(loc), Id(id)
{
}

csaw::IndexExpression::IndexExpression(const SourceLoc& loc, const ExpressionPtr& array, const ExpressionPtr& index)
    : Expression(loc), Array(array), Index(index)
{
}

csaw::IntExpression::IntExpression(const SourceLoc& loc, const uint64_t value)
    : Expression(loc), Value(value)
{
}

csaw::MemberExpression::MemberExpression(const SourceLoc& loc, const ExpressionPtr& object, const std::string& member, const bool deref)
    : Expression(loc), Object(object), Member(member), ShouldDeref(deref)
{
}

csaw::ReferenceExpression::ReferenceExpression(const SourceLoc& loc, const ExpressionPtr& value)
    : Expression(loc), Value(value)
{
}

csaw::SelectExpression::SelectExpression(const SourceLoc& loc, const ExpressionPtr& condition, const ExpressionPtr& _true, const ExpressionPtr& _false)
    : Expression(loc), Condition(condition), True(_true), False(_false)
{
}

csaw::SizeOfExpression::SizeOfExpression(const SourceLoc& loc, const TypePtr& type)
    : Expression(loc), Type(type)
{
}

csaw::StringExpression::StringExpression(const SourceLoc& loc, const std::string& value)
    : Expression(loc), Value(value)
{
}

csaw::UnaryExpression::UnaryExpression(const SourceLoc& loc, const std::string& op, const ExpressionPtr& value, const bool rightop)
    : Expression(loc), Operator(op), Value(value), OpRight(rightop)
{
}

csaw::VarArgExpression::VarArgExpression(const SourceLoc& loc, const TypePtr& type)
    : Expression(loc), Type(type)
{
}
