#pragma once

#include <vector>
#include <csaw/lang/Def.hpp>
#include <csaw/lang/Stmt.hpp>

namespace csaw
{
    struct Expression : Statement
    {
        explicit Expression(size_t line);

        [[nodiscard]] virtual TypePtr GetType() const = 0;
    };

    struct BinaryExpression : Expression
    {
        BinaryExpression(size_t line, const std::string& op, const ExpressionPtr& left, const ExpressionPtr& right);

        [[nodiscard]] TypePtr GetType() const override;

        std::string Operator;
        ExpressionPtr Left;
        ExpressionPtr Right;
    };

    struct CallExpression : Expression
    {
        CallExpression(size_t line, const ExpressionPtr& callee, const std::vector<ExpressionPtr>& args);

        [[nodiscard]] TypePtr GetType() const override;

        ExpressionPtr Callee;
        std::vector<ExpressionPtr> Args;
    };

    struct CastExpression : Expression
    {
        CastExpression(size_t line, const TypePtr& type, const ExpressionPtr& value);

        [[nodiscard]] TypePtr GetType() const override;

        TypePtr Type;
        ExpressionPtr Value;
    };

    struct CharExpression : Expression
    {
        CharExpression(size_t line, char value);
        CharExpression(size_t line, const std::string& value);

        [[nodiscard]] TypePtr GetType() const override;

        char Value;
    };

    struct FloatExpression : Expression
    {
        FloatExpression(size_t line, const std::string& value);

        [[nodiscard]] TypePtr GetType() const override;

        double Value;
    };

    struct IdentifierExpression : Expression
    {
        IdentifierExpression(size_t line, const std::string& id);

        [[nodiscard]] TypePtr GetType() const override;

        std::string Id;
    };

    struct IndexExpression : Expression
    {
        IndexExpression(size_t line, const ExpressionPtr& array, const ExpressionPtr& index);

        [[nodiscard]] TypePtr GetType() const override;

        ExpressionPtr Array;
        ExpressionPtr Index;
    };

    struct IntExpression : Expression
    {
        IntExpression(size_t line, const std::string& value, int base);

        [[nodiscard]] TypePtr GetType() const override;

        uint64_t Value;
    };

    struct MemberExpression : Expression
    {
        MemberExpression(size_t line, const ExpressionPtr& object, const std::string& member);

        [[nodiscard]] TypePtr GetType() const override;

        ExpressionPtr Object;
        std::string Member;
    };

    struct SelectExpression : Expression
    {
        SelectExpression(size_t line, const ExpressionPtr& condition, const ExpressionPtr& _true, const ExpressionPtr& _false);

        [[nodiscard]] TypePtr GetType() const override;

        ExpressionPtr Condition;
        ExpressionPtr True;
        ExpressionPtr False;
    };

    struct StringExpression : Expression
    {
        StringExpression(size_t line, const std::string& value);

        [[nodiscard]] TypePtr GetType() const override;

        std::string Value;
    };

    struct VarArgExpression : Expression
    {
        VarArgExpression(size_t line, const TypePtr& type);

        [[nodiscard]] TypePtr GetType() const override;

        TypePtr Type;
    };

    struct UnaryExpression : Expression
    {
        UnaryExpression(size_t line, const std::string& op, const ExpressionPtr& value, bool rightop);

        [[nodiscard]] TypePtr GetType() const override;

        std::string Operator;
        ExpressionPtr Value;
        bool RightOp;
    };
}
