#include <csaw/Type.hpp>
#include <csaw/lang/Stmt.hpp>

csaw::Statement::Statement(const size_t line)
    : Line(line)
{
}

csaw::AliasStmt::AliasStmt(const size_t line, const std::string& name, const TypePtr& origin)
    : Statement(line), Name(name), Origin(origin)
{
}

csaw::ScopeStatement::ScopeStatement(const size_t line, const std::vector<StatementPtr>& content)
    : Statement(line), Body(content)
{
}

csaw::ForStatement::ForStatement(const size_t line, const StatementPtr& pre, const ExpressionPtr& condition, const StatementPtr& loop, const StatementPtr& body)
    : Statement(line), Pre(pre), Condition(condition), Loop(loop), Body(body)
{
}

csaw::FunctionStatement::FunctionStatement(const size_t line, const bool constructor, const std::string& name, const TypePtr& callee, const std::vector<std::pair<std::string, TypePtr>>& args, const std::string& vararg, const TypePtr& result, const StatementPtr& body)
    : Statement(line), Constructor(constructor), Name(name), Callee(callee), Args(args), VarArg(vararg), Result(result), Body(body)
{
}

csaw::IfStatement::IfStatement(const size_t line, const ExpressionPtr& condition, const StatementPtr& _true, const StatementPtr& _false)
    : Statement(line), Condition(condition), True(_true), False(_false)
{
}

csaw::RetStatement::RetStatement(const size_t line, const ExpressionPtr& value)
    : Statement(line), Value(value)
{
}

csaw::DefStatement::DefStatement(const size_t line, const std::string& name)
    : Statement(line), Name(name)
{
}

csaw::DefStatement::DefStatement(const size_t line, const std::string& name, const TypePtr& origin)
    : Statement(line), Name(name), Origin(origin)
{
}

csaw::DefStatement::DefStatement(const size_t line, const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements)
    : Statement(line), Name(name), Elements(elements)
{
}

csaw::VariableStatement::VariableStatement(const size_t line, const std::string& name, const TypePtr& type, const ExpressionPtr& value)
    : Statement(line), Name(name), Type(type), Value(value)
{
}

csaw::WhileStatement::WhileStatement(const size_t line, const ExpressionPtr& condition, const StatementPtr& body)
    : Statement(line), Condition(condition), Body(body)
{
}
