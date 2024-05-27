#pragma once

#include <vector>
#include <csaw/Def.hpp>
#include <csaw/Stmt.hpp>

namespace csaw
{
    struct Expression : Statement
    {
        explicit Expression(const SourceLoc& loc);
    };

    struct BinaryExpression : Expression
    {
        BinaryExpression(const SourceLoc& loc, const std::string& op, const ExpressionPtr& left, const ExpressionPtr& right);

        std::string Operator;
        ExpressionPtr Left;
        ExpressionPtr Right;
    };

    struct CallExpression : Expression
    {
        CallExpression(const SourceLoc& loc, const ExpressionPtr& callee, const std::vector<TypePtr>& temp_args, const std::vector<ExpressionPtr>& args);

        ExpressionPtr Callee;
        std::vector<TypePtr> TempArgs;
        std::vector<ExpressionPtr> Args;
    };

    struct CastExpression : Expression
    {
        CastExpression(const SourceLoc& loc, const TypePtr& type, const ExpressionPtr& value);

        TypePtr Type;
        ExpressionPtr Value;
    };

    struct CharExpression : Expression
    {
        CharExpression(const SourceLoc& loc, char value);
        CharExpression(const SourceLoc& loc, const std::string& value);

        char Value;
    };

    struct DereferenceExpression : Expression
    {
        DereferenceExpression(const SourceLoc& loc, const ExpressionPtr& value);

        ExpressionPtr Value;
    };

    struct FloatExpression : Expression
    {
        FloatExpression(const SourceLoc& loc, const std::string& value);

        double Value;
    };

    struct IdentifierExpression : Expression
    {
        IdentifierExpression(const SourceLoc& loc, const std::string& id);

        std::string Id;
    };

    struct IndexExpression : Expression
    {
        IndexExpression(const SourceLoc& loc, const ExpressionPtr& array, const ExpressionPtr& index);

        ExpressionPtr Array;
        ExpressionPtr Index;
    };

    struct IntExpression : Expression
    {
        IntExpression(const SourceLoc& loc, uint64_t value);

        uint64_t Value;
    };

    struct MemberExpression : Expression
    {
        MemberExpression(const SourceLoc& loc, const ExpressionPtr& object, const std::string& member, bool deref);

        ExpressionPtr Object;
        std::string Member;
        bool ShouldDeref;
    };

    struct ReferenceExpression : Expression
    {
        ReferenceExpression(const SourceLoc& loc, const ExpressionPtr& value);

        ExpressionPtr Value;
    };

    struct SelectExpression : Expression
    {
        SelectExpression(const SourceLoc& loc, const ExpressionPtr& condition, const ExpressionPtr& _true, const ExpressionPtr& _false);

        ExpressionPtr Condition;
        ExpressionPtr True;
        ExpressionPtr False;
    };

    struct SizeOfExpression : Expression
    {
        SizeOfExpression(const SourceLoc& loc, const TypePtr& type);

        TypePtr Type;
    };

    struct StringExpression : Expression
    {
        StringExpression(const SourceLoc& loc, const std::string& value);

        std::string Value;
    };

    struct VarArgExpression : Expression
    {
        VarArgExpression(const SourceLoc& loc, const TypePtr& type);

        TypePtr Type;
    };

    struct UnaryExpression : Expression
    {
        UnaryExpression(const SourceLoc& loc, const std::string& op, const ExpressionPtr& value, bool rightop);

        std::string Operator;
        ExpressionPtr Value;
        bool OpRight;
    };
}
