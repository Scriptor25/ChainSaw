#pragma once

#include <lang/ASTDef.h>
#include <lang/Stmt.h>

#include <string>
#include <vector>

namespace csaw::lang
{
	struct Expr : Stmt
	{
		virtual ~Expr() {}
	};

	struct CallExpr : Expr
	{
		static CallExprPtr Ptr(const ExprPtr callee, const std::vector<ExprPtr>& args);
		CallExpr(const ExprPtr callee, const std::vector<ExprPtr>& args);

		const ExprPtr Callee;
		const std::vector<ExprPtr> Args;
	};

	struct NumExpr : Expr
	{
		static NumExprPtr Ptr(const std::string& value, const int radix);
		NumExpr(const double value);

		const double Value;
	};

	struct ChrExpr : Expr
	{
		static ChrExprPtr Ptr(const std::string& value);
		ChrExpr(const char value);

		const char Value;
	};

	struct StrExpr : Expr
	{
		static StrExprPtr Ptr(const std::string& value);
		StrExpr(const std::string& value);

		const std::string Value;
	};

	struct IdentExpr : Expr
	{
		static IdentExprPtr Ptr(const std::string& id);
		IdentExpr(const std::string& id);

		const std::string Id;
	};

	struct BinExpr : Expr
	{
		static BinExprPtr Ptr(const std::string& op, const ExprPtr left, const ExprPtr right);
		BinExpr(const std::string& op, const ExprPtr left, const ExprPtr right);

		const std::string Operator;
		const ExprPtr Left;
		const ExprPtr Right;
	};

	struct UnExpr : Expr
	{
		static UnExprPtr Ptr(const std::string& op, const ExprPtr value, const bool rightop = false);
		UnExpr(const std::string& op, const ExprPtr value, const bool rightop);

		const std::string Operator;
		const ExprPtr Value;
		const bool RightOp;
	};

	struct IndexExpr : Expr
	{
		static IndexExprPtr Ptr(const ExprPtr array, const ExprPtr index);
		IndexExpr(const ExprPtr array, const ExprPtr index);

		const ExprPtr Array;
		const ExprPtr Index;
	};

	struct MemberExpr : Expr
	{
		static MemberExprPtr Ptr(const ExprPtr object, const std::string& member);
		MemberExpr(const ExprPtr object, const std::string& member);

		const ExprPtr Object;
		const std::string Member;
	};

	struct VarArgExpr : Expr
	{
		static VarArgExprPtr Ptr(const std::string& type, const ExprPtr index);
		VarArgExpr(const std::string& type, const ExprPtr index);

		const std::string Type;
		const ExprPtr Index;
	};

	struct SelExpr : Expr
	{
		static SelExprPtr Ptr(const ExprPtr condition, const ExprPtr _true, const ExprPtr _false);
		SelExpr(const ExprPtr condition, const ExprPtr _true, const ExprPtr _false);

		const ExprPtr Condition;
		const ExprPtr True;
		const ExprPtr False;
	};
}
