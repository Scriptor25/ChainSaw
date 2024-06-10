#pragma once

#include <map>
#include <vector>
#include <csaw/Def.hpp>
#include <csaw/Stmt.hpp>

namespace csaw
{
    struct Expression : Statement
    {
        explicit Expression(const SourceLoc& loc);

        virtual TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) = 0;
    };

    struct BinaryExpression : Expression
    {
        BinaryExpression(const SourceLoc& loc, const std::string& op, const ExpressionPtr& left, const ExpressionPtr& right);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        std::string Operator;
        ExpressionPtr Left;
        ExpressionPtr Right;
    };

    struct CallExpression : Expression
    {
        CallExpression(const SourceLoc& loc, const ExpressionPtr& callee, const std::vector<TypePtr>& temp_args, const std::vector<ExpressionPtr>& args);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        ExpressionPtr Callee;
        std::vector<TypePtr> TempArgs;
        std::vector<ExpressionPtr> Args;
    };

    struct CastExpression : Expression
    {
        CastExpression(const SourceLoc& loc, const TypePtr& type, const ExpressionPtr& value);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        TypePtr Type;
        ExpressionPtr Value;
    };

    struct CharExpression : Expression
    {
        CharExpression(const SourceLoc& loc, char value);
        CharExpression(const SourceLoc& loc, const std::string& value);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        char Value;
    };

    struct DereferenceExpression : Expression
    {
        DereferenceExpression(const SourceLoc& loc, const ExpressionPtr& value);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        ExpressionPtr Value;
    };

    struct FloatExpression : Expression
    {
        FloatExpression(const SourceLoc& loc, const std::string& value);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        double Value;
    };

    struct IdentifierExpression : Expression
    {
        IdentifierExpression(const SourceLoc& loc, const std::string& id);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        std::string Id;
    };

    struct IndexExpression : Expression
    {
        IndexExpression(const SourceLoc& loc, const ExpressionPtr& array, const ExpressionPtr& index);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        ExpressionPtr Array;
        ExpressionPtr Index;
    };

    struct IntExpression : Expression
    {
        IntExpression(const SourceLoc& loc, uint64_t value);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        uint64_t Value;
    };

    struct MemberExpression : Expression
    {
        MemberExpression(const SourceLoc& loc, const ExpressionPtr& object, const std::string& member, bool deref);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        ExpressionPtr Object;
        std::string Member;
        bool ShouldDeref;
    };

    struct ReferenceExpression : Expression
    {
        ReferenceExpression(const SourceLoc& loc, const ExpressionPtr& value);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        ExpressionPtr Value;
    };

    struct SelectExpression : Expression
    {
        SelectExpression(const SourceLoc& loc, const ExpressionPtr& condition, const ExpressionPtr& _true, const ExpressionPtr& _false);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        ExpressionPtr Condition;
        ExpressionPtr True;
        ExpressionPtr False;
    };

    struct SizeOfExpression : Expression
    {
        SizeOfExpression(const SourceLoc& loc, const TypePtr& type);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        TypePtr Type;
    };

    struct StringExpression : Expression
    {
        StringExpression(const SourceLoc& loc, const std::string& value);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        std::string Value;
    };

    struct UnaryExpression : Expression
    {
        UnaryExpression(const SourceLoc& loc, const std::string& op, const ExpressionPtr& value, bool rightop);

        TypePtr GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args) override;

        std::string Operator;
        ExpressionPtr Value;
        bool OpRight;
    };
}
