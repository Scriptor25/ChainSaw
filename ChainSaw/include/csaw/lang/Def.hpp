#pragma once

#include <memory>

namespace csaw
{
    struct Statement;
    struct ScopeStatement;
    struct ForStatement;
    struct FunctionStatement;
    struct IfStatement;
    struct RetStatement;
    struct DefStatement;
    struct VariableStatement;
    struct WhileStatement;

    typedef std::shared_ptr<Statement> StatementPtr;
    typedef std::shared_ptr<ScopeStatement> ScopeStatementPtr;
    typedef std::shared_ptr<ForStatement> ForStatementPtr;
    typedef std::shared_ptr<FunctionStatement> FunctionStatementPtr;
    typedef std::shared_ptr<IfStatement> IfStatementPtr;
    typedef std::shared_ptr<RetStatement> RetStatementPtr;
    typedef std::shared_ptr<DefStatement> DefStatementPtr;
    typedef std::shared_ptr<VariableStatement> VariableStatementPtr;
    typedef std::shared_ptr<WhileStatement> WhileStatementPtr;

    std::ostream& operator<<(std::ostream& out, const StatementPtr& ptr);
    std::ostream& operator<<(std::ostream& out, const ScopeStatement& statement);
    std::ostream& operator<<(std::ostream& out, const ForStatement& statement);
    std::ostream& operator<<(std::ostream& out, const FunctionStatement& statement);
    std::ostream& operator<<(std::ostream& out, const IfStatement& statement);
    std::ostream& operator<<(std::ostream& out, const RetStatement& statement);
    std::ostream& operator<<(std::ostream& out, const DefStatement& statement);
    std::ostream& operator<<(std::ostream& out, const VariableStatement& statement);
    std::ostream& operator<<(std::ostream& out, const WhileStatement& statement);

    struct Expression;
    struct BinaryExpression;
    struct CallExpression;
    struct CastExpression;
    struct CharExpression;
    struct FloatExpression;
    struct IdentifierExpression;
    struct IndexExpression;
    struct IntExpression;
    struct MemberExpression;
    struct SelectExpression;
    struct StringExpression;
    struct UnaryExpression;
    struct VarArgExpression;

    typedef std::shared_ptr<Expression> ExpressionPtr;
    typedef std::shared_ptr<BinaryExpression> BinaryExpressionPtr;
    typedef std::shared_ptr<CallExpression> CallExpressionPtr;
    typedef std::shared_ptr<CastExpression> CastExpressionPtr;
    typedef std::shared_ptr<CharExpression> CharExpressionPtr;
    typedef std::shared_ptr<IdentifierExpression> IdentifierExpressionPtr;
    typedef std::shared_ptr<IndexExpression> IndexExpressionPtr;
    typedef std::shared_ptr<MemberExpression> MemberExpressionPtr;
    typedef std::shared_ptr<SelectExpression> SelectExpressionPtr;
    typedef std::shared_ptr<StringExpression> StringExpressionPtr;
    typedef std::shared_ptr<UnaryExpression> UnaryExpressionPtr;
    typedef std::shared_ptr<VarArgExpression> VarArgExpressionPtr;

    std::ostream& operator<<(std::ostream& out, const ExpressionPtr& ptr);
    std::ostream& operator<<(std::ostream& out, const BinaryExpression& expression);
    std::ostream& operator<<(std::ostream& out, const CallExpression& expression);
    std::ostream& operator<<(std::ostream& out, const CastExpression& expression);
    std::ostream& operator<<(std::ostream& out, const CharExpression& expression);
    std::ostream& operator<<(std::ostream& out, const FloatExpression& expression);
    std::ostream& operator<<(std::ostream& out, const IdentifierExpression& expression);
    std::ostream& operator<<(std::ostream& out, const IndexExpression& expression);
    std::ostream& operator<<(std::ostream& out, const IntExpression& expression);
    std::ostream& operator<<(std::ostream& out, const MemberExpression& expression);
    std::ostream& operator<<(std::ostream& out, const SelectExpression& expression);
    std::ostream& operator<<(std::ostream& out, const StringExpression& expression);
    std::ostream& operator<<(std::ostream& out, const UnaryExpression& expression);
    std::ostream& operator<<(std::ostream& out, const VarArgExpression& expression);
}
