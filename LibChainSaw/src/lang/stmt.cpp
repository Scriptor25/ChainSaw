#include <csaw/CSaw.hpp>
#include <csaw/Type.hpp>
#include <csaw/lang/Stmt.hpp>

csaw::Statement::Statement(const std::string& filename, const size_t line)
    : Filename(filename), Line(line)
{
}

csaw::AliasStmt::AliasStmt(const std::string& filename, const size_t line, const std::string& name, const TypePtr& origin)
    : Statement(filename, line), Name(name), Origin(origin)
{
}

csaw::ScopeStatement::ScopeStatement(const std::string& filename, const size_t line, const std::vector<StatementPtr>& content)
    : Statement(filename, line), Body(content)
{
}

csaw::ForStatement::ForStatement(const std::string& filename, const size_t line, const StatementPtr& pre, const ExpressionPtr& condition, const StatementPtr& loop, const StatementPtr& body)
    : Statement(filename, line), Pre(pre), Condition(condition), Loop(loop), Body(body)
{
}

csaw::FunctionStatement::FunctionStatement(const std::string& filename, const size_t line, const std::string& name, const TypePtr& parent, const TypePtr& result, const std::vector<std::string>& mods, const std::vector<std::pair<std::string, TypePtr>>& args, const bool is_varargs, const StatementPtr& body)
    : Statement(filename, line), Name(name), Parent(parent), Result(result), Mods(mods), Args(args), IsVarArgs(is_varargs), Body(body)
{
    if (Name.empty())
        CSAW_MESSAGE_STMT(true, *this, "Function name must not be empty");
    if (Name.find('$') != std::string::npos)
        CSAW_MESSAGE_STMT(true, *this, "Function name must not contain '$'");

    if (Parent && !Result)
        CSAW_MESSAGE_STMT(true, *this, "Member function must have return type for consistency reasons");
}

bool csaw::FunctionStatement::IsConstructor() const
{
    return !Parent && !Result;
}

csaw::IfStatement::IfStatement(const std::string& filename, const size_t line, const ExpressionPtr& condition, const StatementPtr& _true, const StatementPtr& _false)
    : Statement(filename, line), Condition(condition), True(_true), False(_false)
{
}

csaw::RetStatement::RetStatement(const std::string& filename, const size_t line, const ExpressionPtr& value)
    : Statement(filename, line), Value(value)
{
}

csaw::DefStatement::DefStatement(const std::string& filename, const size_t line, const std::string& name)
    : Statement(filename, line), Name(name)
{
}

csaw::DefStatement::DefStatement(const std::string& filename, const size_t line, const std::string& name, const TypePtr& origin)
    : Statement(filename, line), Name(name), Origin(origin)
{
}

csaw::DefStatement::DefStatement(const std::string& filename, const size_t line, const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements)
    : Statement(filename, line), Name(name), Elements(elements)
{
}

csaw::VariableStatement::VariableStatement(const std::string& filename, const size_t line, const std::string& name, const TypePtr& type, const ExpressionPtr& value)
    : Statement(filename, line), Name(name), Type(type), Value(value)
{
}

csaw::WhileStatement::WhileStatement(const std::string& filename, const size_t line, const ExpressionPtr& condition, const StatementPtr& body)
    : Statement(filename, line), Condition(condition), Body(body)
{
}
