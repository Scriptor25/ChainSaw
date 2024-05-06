#pragma once

#include <vector>
#include <csaw/lang/Def.hpp>
#include <csaw/lang/Stmt.hpp>

namespace csaw
{
    struct Expression : Statement
    {
        explicit Expression(size_t line);
    };

    struct BinaryExpression : Expression
    {
        BinaryExpression(size_t line, const std::string& op, const ExpressionPtr& left, const ExpressionPtr& right);

        std::string Operator;
        ExpressionPtr Left;
        ExpressionPtr Right;
    };

    struct CallExpression : Expression
    {
        CallExpression(size_t line, const ExpressionPtr& callee, const std::vector<ExpressionPtr>& args);

        ExpressionPtr Callee;
        std::vector<ExpressionPtr> Args;
    };

    struct CastExpression : Expression
    {
        CastExpression(size_t line, const TypePtr& type, const ExpressionPtr& value);

        TypePtr Type;
        ExpressionPtr Value;
    };

    struct CharExpression : Expression
    {
        CharExpression(size_t line, char value);
        CharExpression(size_t line, const std::string& value);

        char Value;
    };

    struct FloatExpression : Expression
    {
        FloatExpression(size_t line, const std::string& value);

        double Value;
    };

    struct IdentifierExpression : Expression
    {
        IdentifierExpression(size_t line, const std::string& id);

        std::string Id;
    };

    struct IndexExpression : Expression
    {
        IndexExpression(size_t line, const ExpressionPtr& array, const ExpressionPtr& index);

        ExpressionPtr Array;
        ExpressionPtr Index;
    };

    struct IntExpression : Expression
    {
        IntExpression(size_t line, const std::string& value, int base);

        uint32_t Value;
    };

    struct MemberExpression : Expression
    {
        MemberExpression(size_t line, const ExpressionPtr& object, const std::string& member);

        ExpressionPtr Object;
        std::string Member;
    };

    struct SelectExpression : Expression
    {
        SelectExpression(size_t line, const ExpressionPtr& condition, const ExpressionPtr& _true, const ExpressionPtr& _false);

        ExpressionPtr Condition;
        ExpressionPtr True;
        ExpressionPtr False;
    };

    struct StringExpression : Expression
    {
        StringExpression(size_t line, const std::string& value);

        std::string Value;
    };

    struct VarArgExpression : Expression
    {
        VarArgExpression(size_t line, const TypePtr& type);

        TypePtr Type;
    };

    struct UnaryExpression : Expression
    {
        UnaryExpression(size_t line, const std::string& op, const ExpressionPtr& value, bool rightop);

        std::string Operator;
        ExpressionPtr Value;
        bool OpRight;
    };
}
