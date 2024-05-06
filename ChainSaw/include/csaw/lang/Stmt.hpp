#pragma once

#include <vector>
#include <csaw/Type.hpp>
#include <csaw/lang/Def.hpp>

namespace csaw
{
    struct Statement
    {
        explicit Statement(size_t line);
        virtual ~Statement() = default;

        size_t Line;
    };

    struct AliasStmt : Statement
    {
        AliasStmt(size_t line, const std::string& name, const TypePtr& origin);

        std::string Name;
        TypePtr Origin;
    };

    struct ScopeStatement : Statement
    {
        ScopeStatement(size_t line, const std::vector<StatementPtr>& content);

        std::vector<StatementPtr> Body;
    };

    struct ForStatement : Statement
    {
        ForStatement(size_t line, const StatementPtr& pre, const ExpressionPtr& condition, const StatementPtr& loop, const StatementPtr& body);

        StatementPtr Pre;
        ExpressionPtr Condition;
        StatementPtr Loop;
        StatementPtr Body;
    };

    struct FunctionStatement : Statement
    {
        FunctionStatement(size_t line, bool constructor, const std::string& name, const TypePtr& callee, const std::vector<std::pair<std::string, TypePtr>>& args, bool vararg, const TypePtr& result, const StatementPtr& body);

        bool Constructor;
        std::string Name;
        TypePtr Callee;
        std::vector<std::pair<std::string, TypePtr>> Args;
        bool VarArg;
        TypePtr Result;
        StatementPtr Body;
    };

    struct IfStatement : Statement
    {
        IfStatement(size_t line, const ExpressionPtr& condition, const StatementPtr& _true, const StatementPtr& _false);

        ExpressionPtr Condition;
        StatementPtr True;
        StatementPtr False;
    };

    struct RetStatement : Statement
    {
        RetStatement(size_t line, const ExpressionPtr& value);

        ExpressionPtr Value;
    };

    struct DefStatement : Statement
    {
        DefStatement(size_t line, const std::string& name);
        DefStatement(size_t line, const std::string& name, const TypePtr& origin);
        DefStatement(size_t line, const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements);

        std::string Name;
        std::vector<std::pair<std::string, TypePtr>> Elements;
        TypePtr Origin;
    };

    struct VariableStatement : Statement
    {
        VariableStatement(size_t line, const std::string& name, const TypePtr& type, const ExpressionPtr& value);

        std::string Name;
        TypePtr Type;
        ExpressionPtr Value;
    };

    struct WhileStatement : Statement
    {
        WhileStatement(size_t line, const ExpressionPtr& condition, const StatementPtr& body);

        ExpressionPtr Condition;
        StatementPtr Body;
    };
}
