#pragma once

#include <lang/ASTDef.h>

#include <memory>
#include <string>
#include <vector>

namespace csaw::lang
{
	struct Stmt
	{
		virtual ~Stmt() {}
	};

	struct FunStmt : Stmt
	{
		static FunStmtPtr Ptr(const bool constructor, const std::string& name, const std::string& result, const std::vector<std::pair<std::string, std::string>>& params, const bool vararg, const std::string& callee, const EnclosedStmtPtr body);
		static FunStmtPtr Ptr(const bool constructor, const std::string& name, const std::string& result, const std::vector<std::pair<std::string, std::string>>& params, const bool vararg, const std::string& callee);
		FunStmt(const bool constructor, const std::string& name, const std::string& result, const std::vector<std::pair<std::string, std::string>>& params, const bool vararg, const std::string& callee, const EnclosedStmtPtr body);

		const bool Constructor;
		const std::string Name;
		const std::string Result;
		const std::vector<std::pair<std::string, std::string>> Params;
		const bool VarArg;
		const std::string Callee;
		const EnclosedStmtPtr Body;
	};

	struct RetStmt : Stmt
	{
		static RetStmtPtr Ptr(const ExprPtr value);
		static RetStmtPtr Ptr();
		RetStmt(const ExprPtr value);

		const ExprPtr Value;
	};

	struct EnclosedStmt : Stmt
	{
		static EnclosedStmtPtr Ptr(const std::vector<StmtPtr>& content);
		EnclosedStmt(const std::vector<StmtPtr>& content);

		const std::vector<StmtPtr> Content;
	};

	struct ForStmt : Stmt
	{
		static ForStmtPtr Ptr(const StmtPtr pre, const ExprPtr condition, const StmtPtr loop, const StmtPtr body);
		ForStmt(const StmtPtr pre, const ExprPtr condition, const StmtPtr loop, const StmtPtr body);

		const StmtPtr Pre;
		const ExprPtr Condition;
		const StmtPtr Loop;
		const StmtPtr Body;
	};

	struct VarStmt : Stmt
	{
		static VarStmtPtr Ptr(const std::string& type, const std::string& name, const ExprPtr value);
		static VarStmtPtr Ptr(const std::string& type, const std::string& name);
		VarStmt(const std::string& type, const std::string& name, const ExprPtr value);

		const std::string Type;
		const std::string Name;
		const ExprPtr Value;
	};

	struct IncStmt : Stmt
	{
		static IncStmtPtr Ptr(const std::string& filename);
		IncStmt(const std::string& filename);

		const std::string Filename;
	};

	struct WhileStmt : Stmt
	{
		static WhileStmtPtr Ptr(const ExprPtr condition, const StmtPtr body);
		WhileStmt(const ExprPtr condition, const StmtPtr body);

		const ExprPtr Condition;
		const StmtPtr Body;
	};

	struct IfStmt : Stmt
	{
		static IfStmtPtr Ptr(const ExprPtr condition, const StmtPtr _true, const StmtPtr _false);
		static IfStmtPtr Ptr(const ExprPtr condition, const StmtPtr _true);
		IfStmt(const ExprPtr condition, const StmtPtr _true, const StmtPtr _false);

		const ExprPtr Condition;
		const StmtPtr True;
		const StmtPtr False;
	};

	struct ThingStmt : Stmt
	{
		static ThingStmtPtr Ptr(const std::string& name, const std::string& group, const std::vector<std::pair<std::string, std::string>>& elements);
		static ThingStmtPtr Ptr(const std::string& name, const std::string& group);
		ThingStmt(const std::string& name, const std::string& group, const std::vector<std::pair<std::string, std::string>>& elements);

		const std::string Name;
		const std::string Group;
		const std::vector<std::pair<std::string, std::string>> Elements;
	};

	struct AliasStmt : Stmt
	{
		static AliasStmtPtr Ptr(const std::string& name, const std::string& origin);
		AliasStmt(const std::string& name, const std::string& origin);

		const std::string Name;
		const std::string Origin;
	};
}
