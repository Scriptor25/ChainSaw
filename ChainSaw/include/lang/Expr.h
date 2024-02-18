#pragma once

#include <lang/Def.h>
#include <lang/Stmt.h>

#include <string>
#include <vector>

namespace csaw::lang
{
	struct Expr : Stmt
	{
		Expr(size_t line);
	};

	struct CallExpr : Expr
	{
		CallExpr(size_t line, ExprPtr callee, const std::vector<ExprPtr>& args);

		ExprPtr Callee;
		std::vector<ExprPtr> Args;
	};

	struct NumExpr : Expr
	{
		NumExpr(size_t line, double value);
		NumExpr(size_t line, const std::string& value, int base);

		double Value;
	};

	struct ChrExpr : Expr
	{
		ChrExpr(size_t line, char value);
		ChrExpr(size_t line, const std::string& value);

		char Value;
	};

	struct StrExpr : Expr
	{
		StrExpr(size_t line, const std::string& value);

		std::string Value;
	};

	struct IdentExpr : Expr
	{
		IdentExpr(size_t line, const std::string& id);

		std::string Id;
	};

	struct BinExpr : Expr
	{
		BinExpr(size_t line, const std::string& op, ExprPtr left, ExprPtr right);

		std::string Operator;
		ExprPtr Left;
		ExprPtr Right;
	};

	struct UnExpr : Expr
	{
		UnExpr(size_t line, const std::string& op, ExprPtr value, bool rightop);

		std::string Operator;
		ExprPtr Value;
		bool RightOp;
	};

	struct IndexExpr : Expr
	{
		IndexExpr(size_t line, ExprPtr array, ExprPtr index);

		ExprPtr Array;
		ExprPtr Index;
	};

	struct MemberExpr : Expr
	{
		MemberExpr(size_t line, ExprPtr object, const std::string& member);

		ExprPtr Object;
		std::string Member;
	};

	struct VarArgExpr : Expr
	{
		VarArgExpr(size_t line, const std::string& type, ExprPtr index);

		std::string Type;
		ExprPtr Index;
	};

	struct SelExpr : Expr
	{
		SelExpr(size_t line, ExprPtr condition, ExprPtr _true, ExprPtr _false);

		ExprPtr Condition;
		ExprPtr True;
		ExprPtr False;
	};
}
