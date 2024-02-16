#pragma once

#include <memory>
#include <iostream>

namespace csaw::lang
{
	struct Stmt;
	struct FunStmt;
	struct RetStmt;
	struct EnclosedStmt;
	struct ForStmt;
	struct VarStmt;
	struct IncStmt;
	struct WhileStmt;
	struct IfStmt;
	struct ThingStmt;

	typedef std::shared_ptr<Stmt> StmtPtr;
	typedef std::shared_ptr<FunStmt> FunStmtPtr;
	typedef std::shared_ptr<RetStmt> RetStmtPtr;
	typedef std::shared_ptr<EnclosedStmt> EnclosedStmtPtr;
	typedef std::shared_ptr<ForStmt> ForStmtPtr;
	typedef std::shared_ptr<VarStmt> VarStmtPtr;
	typedef std::shared_ptr<IncStmt> IncStmtPtr;
	typedef std::shared_ptr<WhileStmt> WhileStmtPtr;
	typedef std::shared_ptr<IfStmt> IfStmtPtr;
	typedef std::shared_ptr<ThingStmt> ThingStmtPtr;

	struct Expr;
	struct CallExpr;
	struct NumExpr;
	struct ChrExpr;
	struct StrExpr;
	struct IdentExpr;
	struct BinExpr;
	struct UnExpr;
	struct IndexExpr;
	struct MemberExpr;
	struct VarArgExpr;
	struct SelExpr;

	typedef std::shared_ptr<Expr> ExprPtr;
	typedef std::shared_ptr<CallExpr> CallExprPtr;
	typedef std::shared_ptr<NumExpr> NumExprPtr;
	typedef std::shared_ptr<ChrExpr> ChrExprPtr;
	typedef std::shared_ptr<StrExpr> StrExprPtr;
	typedef std::shared_ptr<IdentExpr> IdentExprPtr;
	typedef std::shared_ptr<BinExpr> BinExprPtr;
	typedef std::shared_ptr<UnExpr> UnExprPtr;
	typedef std::shared_ptr<IndexExpr> IndexExprPtr;
	typedef std::shared_ptr<MemberExpr> MemberExprPtr;
	typedef std::shared_ptr<VarArgExpr> VarArgExprPtr;
	typedef std::shared_ptr<SelExpr> SelExprPtr;

	std::ostream& operator<<(std::ostream& out, const FunStmt& stmt);
	std::ostream& operator<<(std::ostream& out, const RetStmt& stmt);
	std::ostream& operator<<(std::ostream& out, const EnclosedStmt& stmt);
	std::ostream& operator<<(std::ostream& out, const ForStmt& stmt);
	std::ostream& operator<<(std::ostream& out, const VarStmt& stmt);
	std::ostream& operator<<(std::ostream& out, const StmtPtr ptr);

	std::ostream& operator<<(std::ostream& out, const CallExpr& expr);
	std::ostream& operator<<(std::ostream& out, const NumExpr& expr);
	std::ostream& operator<<(std::ostream& out, const ChrExpr& expr);
	std::ostream& operator<<(std::ostream& out, const StrExpr& expr);
	std::ostream& operator<<(std::ostream& out, const IdentExpr& expr);
	std::ostream& operator<<(std::ostream& out, const BinExpr& expr);
	std::ostream& operator<<(std::ostream& out, const UnExpr& expr);
	std::ostream& operator<<(std::ostream& out, const IndexExpr& expr);
	std::ostream& operator<<(std::ostream& out, const MemberExpr& expr);
	std::ostream& operator<<(std::ostream& out, const VarArgExpr& expr);
	std::ostream& operator<<(std::ostream& out, const ExprPtr ptr);
}
