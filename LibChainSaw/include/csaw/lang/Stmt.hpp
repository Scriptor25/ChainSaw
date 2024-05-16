#pragma once

#include <vector>
#include <csaw/Type.hpp>
#include <csaw/lang/Def.hpp>

namespace csaw
{
    struct Statement
    {
        Statement(const std::string& filename, size_t line);
        virtual ~Statement() = default;

        std::string Filename;
        size_t Line;
    };

    struct AliasStmt : Statement
    {
        AliasStmt(const std::string& filename, size_t line, const std::string& name, const TypePtr& origin);

        std::string Name;
        TypePtr Origin;
    };

    struct ScopeStatement : Statement
    {
        ScopeStatement(const std::string& filename, size_t line, const std::vector<StatementPtr>& content);

        std::vector<StatementPtr> Body;
    };

    struct ForStatement : Statement
    {
        ForStatement(const std::string& filename, size_t line, const StatementPtr& pre, const ExpressionPtr& condition, const StatementPtr& loop, const StatementPtr& body);

        StatementPtr Pre;
        ExpressionPtr Condition;
        StatementPtr Loop;
        StatementPtr Body;
    };

    struct FunctionStatement : Statement
    {
        FunctionStatement(const std::string& filename, size_t line, const std::string& name, const TypePtr& parent, const TypePtr& result, const std::vector<std::string>& mods, const std::vector<std::pair<std::string, TypePtr>>& args, bool is_varargs, const StatementPtr& body);

        bool IsConstructor() const;

        std::string Name;
        TypePtr Parent;
        TypePtr Result;
        std::vector<std::string> Mods;
        std::vector<std::pair<std::string, TypePtr>> Args;
        bool IsVarArgs;
        StatementPtr Body;
    };

    struct IfStatement : Statement
    {
        IfStatement(const std::string& filename, size_t line, const ExpressionPtr& condition, const StatementPtr& _true, const StatementPtr& _false);

        ExpressionPtr Condition;
        StatementPtr True;
        StatementPtr False;
    };

    struct RetStatement : Statement
    {
        RetStatement(const std::string& filename, size_t line, const ExpressionPtr& value);

        ExpressionPtr Value;
    };

    struct DefStatement : Statement
    {
        DefStatement(const std::string& filename, size_t line, const std::string& name);
        DefStatement(const std::string& filename, size_t line, const std::string& name, const TypePtr& origin);
        DefStatement(const std::string& filename, size_t line, const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements);

        std::string Name;
        std::vector<std::pair<std::string, TypePtr>> Elements;
        TypePtr Origin;
    };

    struct VariableStatement : Statement
    {
        VariableStatement(const std::string& filename, size_t line, const std::string& name, const TypePtr& type, const ExpressionPtr& value);

        std::string Name;
        TypePtr Type;
        ExpressionPtr Value;
    };

    struct WhileStatement : Statement
    {
        WhileStatement(const std::string& filename, size_t line, const ExpressionPtr& condition, const StatementPtr& body);

        ExpressionPtr Condition;
        StatementPtr Body;
    };
}
