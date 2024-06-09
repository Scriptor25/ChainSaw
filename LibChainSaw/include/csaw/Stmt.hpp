#pragma once

#include <vector>
#include <csaw/Def.hpp>

namespace csaw
{
    struct Statement
    {
        explicit Statement(const SourceLoc& loc);
        virtual ~Statement();

        SourceLoc Loc;
    };

    struct ScopeStatement : Statement
    {
        ScopeStatement(const SourceLoc& loc, const std::vector<StatementPtr>& content);

        std::vector<StatementPtr> Body;
    };

    struct ForStatement : Statement
    {
        ForStatement(const SourceLoc& loc, const StatementPtr& pre, const ExpressionPtr& condition, const StatementPtr& loop, const StatementPtr& body);

        StatementPtr Pre;
        ExpressionPtr Condition;
        StatementPtr Loop;
        StatementPtr Body;
    };

    struct FunctionStatement : Statement
    {
        FunctionStatement(const SourceLoc& loc, const std::string& name, const TypePtr& parent, const TypePtr& result, const std::vector<std::string>& mods, const std::vector<Arg>& args, bool is_varargs, const StatementPtr& body);

        [[nodiscard]] bool IsConstructor() const;

        std::string Name;
        TypePtr Parent;
        TypePtr Result;
        std::vector<std::string> Mods;
        std::vector<Arg> Args;
        bool IsVarArgs;
        StatementPtr Body;
    };

    struct IfStatement : Statement
    {
        IfStatement(const SourceLoc& loc, const ExpressionPtr& condition, const StatementPtr& _true, const StatementPtr& _false);

        ExpressionPtr Condition;
        StatementPtr True;
        StatementPtr False;
    };

    struct RetStatement : Statement
    {
        RetStatement(const SourceLoc& loc, const ExpressionPtr& value);

        ExpressionPtr Value;
    };

    struct DefStatement : Statement
    {
        DefStatement(const SourceLoc& loc, const std::string& name);
        DefStatement(const SourceLoc& loc, const std::string& name, const TypePtr& origin);
        DefStatement(const SourceLoc& loc, const std::string& name, const std::vector<Arg>& elements);

        std::string Name;
        std::vector<Arg> Elements;
        TypePtr Origin;
    };

    struct VariableStatement : Statement
    {
        VariableStatement(const SourceLoc& loc, const std::string& name, const std::vector<std::string>& mods, const TypePtr& type, const ExpressionPtr& value);

        std::string Name;
        std::vector<std::string> Mods;
        TypePtr Type;
        ExpressionPtr Value;
    };

    struct WhileStatement : Statement
    {
        WhileStatement(const SourceLoc& loc, const ExpressionPtr& condition, const StatementPtr& body);

        ExpressionPtr Condition;
        StatementPtr Body;
    };
}
