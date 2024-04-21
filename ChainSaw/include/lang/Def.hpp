#pragma once

#include <iostream>
#include <memory>

namespace csaw::lang
{
    struct Stmt;
    typedef std::shared_ptr<Stmt> StmtPtr;

    struct FunStmt;
    typedef std::shared_ptr<FunStmt> FunStmtPtr;

    struct RetStmt;
    typedef std::shared_ptr<RetStmt> RetStmtPtr;

    struct EnclosedStmt;
    typedef std::shared_ptr<EnclosedStmt> EnclosedStmtPtr;

    struct ForStmt;
    typedef std::shared_ptr<ForStmt> ForStmtPtr;

    struct VarStmt;
    typedef std::shared_ptr<VarStmt> VarStmtPtr;

    struct WhileStmt;
    typedef std::shared_ptr<WhileStmt> WhileStmtPtr;

    struct IfStmt;
    typedef std::shared_ptr<IfStmt> IfStmtPtr;

    struct ThingStmt;
    typedef std::shared_ptr<ThingStmt> ThingStmtPtr;

    struct AliasStmt;
    typedef std::shared_ptr<AliasStmt> AliasStmtPtr;

    struct Expr;
    typedef std::shared_ptr<Expr> ExprPtr;

    struct CallExpr;
    typedef std::shared_ptr<CallExpr> CallExprPtr;

    struct NumExpr;
    typedef std::shared_ptr<NumExpr> NumExprPtr;

    struct ChrExpr;
    typedef std::shared_ptr<ChrExpr> ChrExprPtr;

    struct StrExpr;
    typedef std::shared_ptr<StrExpr> StrExprPtr;

    struct IdentExpr;
    typedef std::shared_ptr<IdentExpr> IdentExprPtr;

    struct BinExpr;
    typedef std::shared_ptr<BinExpr> BinExprPtr;

    struct UnExpr;
    typedef std::shared_ptr<UnExpr> UnExprPtr;

    struct IndexExpr;
    typedef std::shared_ptr<IndexExpr> IndexExprPtr;

    struct MemberExpr;
    typedef std::shared_ptr<MemberExpr> MemberExprPtr;

    struct VarArgExpr;
    typedef std::shared_ptr<VarArgExpr> VarArgExprPtr;

    struct SelExpr;
    typedef std::shared_ptr<SelExpr> SelExprPtr;

    std::ostream &operator<<(std::ostream &out, const FunStmt &stmt);

    std::ostream &operator<<(std::ostream &out, const RetStmt &stmt);

    std::ostream &operator<<(std::ostream &out, const EnclosedStmt &stmt);

    std::ostream &operator<<(std::ostream &out, const ForStmt &stmt);

    std::ostream &operator<<(std::ostream &out, const VarStmt &stmt);

    std::ostream &operator<<(std::ostream &out, const WhileStmt &stmt);

    std::ostream &operator<<(std::ostream &out, const IfStmt &stmt);

    std::ostream &operator<<(std::ostream &out, const ThingStmt &stmt);

    std::ostream &operator<<(std::ostream &out, const AliasStmt &stmt);

    std::ostream &operator<<(std::ostream &out, StmtPtr ptr);

    std::ostream &operator<<(std::ostream &out, const CallExpr &expr);

    std::ostream &operator<<(std::ostream &out, const NumExpr &expr);

    std::ostream &operator<<(std::ostream &out, const ChrExpr &expr);

    std::ostream &operator<<(std::ostream &out, const StrExpr &expr);

    std::ostream &operator<<(std::ostream &out, const IdentExpr &expr);

    std::ostream &operator<<(std::ostream &out, const BinExpr &expr);

    std::ostream &operator<<(std::ostream &out, const UnExpr &expr);

    std::ostream &operator<<(std::ostream &out, const IndexExpr &expr);

    std::ostream &operator<<(std::ostream &out, const MemberExpr &expr);

    std::ostream &operator<<(std::ostream &out, const VarArgExpr &expr);

    std::ostream &operator<<(std::ostream &out, const SelExpr &expr);

    std::ostream &operator<<(std::ostream &out, ExprPtr ptr);
}
