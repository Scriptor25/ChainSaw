#include <csaw/Error.hpp>
#include <csaw/Stmt.hpp>
#include <csaw/Type.hpp>

csaw::Statement::Statement(const SourceLoc& loc)
    : Loc(loc)
{
}

csaw::Statement::~Statement() = default;

csaw::ScopeStatement::ScopeStatement(const SourceLoc& loc, const std::vector<StatementPtr>& content)
    : Statement(loc), Body(content)
{
}

csaw::ForStatement::ForStatement(const SourceLoc& loc, const StatementPtr& pre, const ExpressionPtr& condition, const StatementPtr& loop, const StatementPtr& body)
    : Statement(loc), Pre(pre), Condition(condition), Loop(loop), Body(body)
{
}

csaw::FunctionStatement::FunctionStatement(const SourceLoc& loc, const std::string& name, const TypePtr& parent, const TypePtr& result, const std::vector<std::string>& mods, const std::vector<Arg>& args, const bool is_varargs, const StatementPtr& body)
    : Statement(loc), Name(name), Parent(parent), Result(result), Mods(mods), Args(args), IsVarArgs(is_varargs), Body(body)
{
    if (Name.empty())
        ThrowErrorStmt(*this, true, "Function name must not be empty");

    if (Name.find('$') != std::string::npos)
        ThrowErrorStmt(*this, true, "Function name must not contain '$'");

    if (Parent && !Result)
    {
        ThrowErrorStmt(*this, false, "Every non-constructor function must always have a result type; defaults to void because a parent is specified, so it cannot be a constructor");
        Result = Type::GetVoid();
    }
}

bool csaw::FunctionStatement::IsConstructor() const
{
    return !Parent && !Result;
}

csaw::IfStatement::IfStatement(const SourceLoc& loc, const ExpressionPtr& condition, const StatementPtr& _true, const StatementPtr& _false)
    : Statement(loc), Condition(condition), True(_true), False(_false)
{
}

csaw::RetStatement::RetStatement(const SourceLoc& loc, const ExpressionPtr& value)
    : Statement(loc), Value(value)
{
}

csaw::DefStatement::DefStatement(const SourceLoc& loc, const std::string& name)
    : Statement(loc), Name(name)
{
}

csaw::DefStatement::DefStatement(const SourceLoc& loc, const std::string& name, const TypePtr& origin)
    : Statement(loc), Name(name), Origin(origin)
{
}

csaw::DefStatement::DefStatement(const SourceLoc& loc, const std::string& name, const std::vector<Arg>& elements)
    : Statement(loc), Name(name), Elements(elements)
{
}

csaw::VariableStatement::VariableStatement(const SourceLoc& loc, const std::string& name, const std::vector<std::string>& mods, const TypePtr& type, const ExpressionPtr& value)
    : Statement(loc), Name(name), Mods(mods), Type(type), Value(value)
{
}

csaw::WhileStatement::WhileStatement(const SourceLoc& loc, const ExpressionPtr& condition, const StatementPtr& body)
    : Statement(loc), Condition(condition), Body(body)
{
}
