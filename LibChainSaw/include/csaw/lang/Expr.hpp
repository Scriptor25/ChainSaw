#pragma once

#include <vector>
#include <csaw/lang/Def.hpp>
#include <csaw/lang/Stmt.hpp>

namespace csaw
{
    struct Expression : Statement
    {
        Expression(const std::string& filename, size_t line);
    };

    struct BinaryExpression : Expression
    {
        BinaryExpression(const std::string& filename, size_t line, const std::string& op, const ExpressionPtr& left, const ExpressionPtr& right);

        std::string Operator;
        ExpressionPtr Left;
        ExpressionPtr Right;
    };

    struct CallExpression : Expression
    {
        CallExpression(const std::string& filename, size_t line, const ExpressionPtr& callee, const std::vector<ExpressionPtr>& args);

        ExpressionPtr Callee;
        std::vector<ExpressionPtr> Args;
    };

    struct CastExpression : Expression
    {
        CastExpression(const std::string& filename, size_t line, const TypePtr& type, const ExpressionPtr& value);

        TypePtr Type;
        ExpressionPtr Value;
    };

    struct CharExpression : Expression
    {
        CharExpression(const std::string& filename, size_t line, char value);
        CharExpression(const std::string& filename, size_t line, const std::string& value);

        char Value;
    };

    struct DereferenceExpression : Expression
    {
        DereferenceExpression(const std::string& filename, size_t line, const ExpressionPtr& value);

        ExpressionPtr Value;
    };

    struct FloatExpression : Expression
    {
        FloatExpression(const std::string& filename, size_t line, const std::string& value);

        double Value;
    };

    struct IdentifierExpression : Expression
    {
        IdentifierExpression(const std::string& filename, size_t line, const std::string& id);

        std::string Id;
    };

    struct IndexExpression : Expression
    {
        IndexExpression(const std::string& filename, size_t line, const ExpressionPtr& array, const ExpressionPtr& index);

        ExpressionPtr Array;
        ExpressionPtr Index;
    };

    struct IntExpression : Expression
    {
        IntExpression(const std::string& filename, size_t line, uint64_t value);

        uint64_t Value;
    };

    struct MemberExpression : Expression
    {
        MemberExpression(const std::string& filename, size_t line, const ExpressionPtr& object, const std::string& member, bool deref);

        ExpressionPtr Object;
        std::string Member;
        bool ShouldDeref;
    };

    struct ReferenceExpression : Expression
    {
        ReferenceExpression(const std::string& filename, size_t line, const ExpressionPtr& value);

        ExpressionPtr Value;
    };

    struct SelectExpression : Expression
    {
        SelectExpression(const std::string& filename, size_t line, const ExpressionPtr& condition, const ExpressionPtr& _true, const ExpressionPtr& _false);

        ExpressionPtr Condition;
        ExpressionPtr True;
        ExpressionPtr False;
    };

    struct SizeOfExpression : Expression
    {
        SizeOfExpression(const std::string& filename, size_t line, const TypePtr& type);

        TypePtr Type;
    };

    struct StringExpression : Expression
    {
        StringExpression(const std::string& filename, size_t line, const std::string& value);

        std::string Value;
    };

    struct VarArgExpression : Expression
    {
        VarArgExpression(const std::string& filename, size_t line, const TypePtr& type);

        TypePtr Type;
    };

    struct UnaryExpression : Expression
    {
        UnaryExpression(const std::string& filename, size_t line, const std::string& op, const ExpressionPtr& value, bool rightop);

        std::string Operator;
        ExpressionPtr Value;
        bool OpRight;
    };
}
