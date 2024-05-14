#include <csaw/CSaw.hpp>
#include <csaw/Type.hpp>
#include <csaw/lang/Stmt.hpp>

csaw::Statement::Statement(const std::string& file, const size_t line)
    : File(file), Line(line)
{
}

csaw::AliasStmt::AliasStmt(const std::string& file, const size_t line, const std::string& name, const TypePtr& origin)
    : Statement(file, line), Name(name), Origin(origin)
{
}

csaw::ScopeStatement::ScopeStatement(const std::string& file, const size_t line, const std::vector<StatementPtr>& content)
    : Statement(file, line), Body(content)
{
}

csaw::ForStatement::ForStatement(const std::string& file, const size_t line, const StatementPtr& pre, const ExpressionPtr& condition, const StatementPtr& loop, const StatementPtr& body)
    : Statement(file, line), Pre(pre), Condition(condition), Loop(loop), Body(body)
{
}

csaw::FunctionStatement::FunctionStatement(const std::string& file, const size_t line, const std::string& name, const TypePtr& callee, const TypePtr& result, const std::vector<std::pair<std::string, TypePtr>>& args, const bool is_constructor, const bool is_vararg, const StatementPtr& body)
    : Statement(file, line), Name(name), Callee(callee), Result(result), Args(args), IsConstructor(is_constructor), IsVarArg(is_vararg), Body(body)
{
    if (Name.empty())
        CSAW_MESSAGE_STMT(true, *this, "function name must not be empty");
    if (IsConstructor)
    {
        if (Callee)
            CSAW_MESSAGE_STMT(true, *this, "a constructor must not be declared as a member function");
        if (Result)
            CSAW_MESSAGE_STMT(true, *this, "a constructor must not have a return type");
    }
    else
    {
        if (!Result) Result = Type::GetVoid();
    }
}

csaw::IfStatement::IfStatement(const std::string& file, const size_t line, const ExpressionPtr& condition, const StatementPtr& _true, const StatementPtr& _false)
    : Statement(file, line), Condition(condition), True(_true), False(_false)
{
}

csaw::RetStatement::RetStatement(const std::string& file, const size_t line, const ExpressionPtr& value)
    : Statement(file, line), Value(value)
{
}

csaw::DefStatement::DefStatement(const std::string& file, const size_t line, const std::string& name)
    : Statement(file, line), Name(name)
{
}

csaw::DefStatement::DefStatement(const std::string& file, const size_t line, const std::string& name, const TypePtr& origin)
    : Statement(file, line), Name(name), Origin(origin)
{
}

csaw::DefStatement::DefStatement(const std::string& file, const size_t line, const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements)
    : Statement(file, line), Name(name), Elements(elements)
{
}

csaw::VariableStatement::VariableStatement(const std::string& file, const size_t line, const std::string& name, const TypePtr& type, const ExpressionPtr& value)
    : Statement(file, line), Name(name), Type(type), Value(value)
{
}

csaw::WhileStatement::WhileStatement(const std::string& file, const size_t line, const ExpressionPtr& condition, const StatementPtr& body)
    : Statement(file, line), Condition(condition), Body(body)
{
}
