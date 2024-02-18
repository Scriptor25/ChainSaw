#pragma once

#include <lang/Def.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace csaw::lang
{
	struct Stmt
	{
		Stmt(size_t line);
		virtual ~Stmt() {}

		size_t Line;
	};

	struct FunStmt : Stmt
	{
		FunStmt(size_t line, bool constructor, const std::string& name, const std::string& result, const std::vector<std::pair<std::string, std::string>>& args, bool vararg, const std::string& callee, EnclosedStmtPtr body);

		bool Constructor;
		std::string Name;
		std::string Result;
		std::vector<std::pair<std::string, std::string>> Args;
		bool VarArg;
		std::string Callee;
		EnclosedStmtPtr Body;
	};

	struct RetStmt : Stmt
	{
		RetStmt(size_t line, ExprPtr value);

		ExprPtr Value;
	};

	struct EnclosedStmt : Stmt
	{
		EnclosedStmt(size_t line, const std::vector<StmtPtr>& content);

		std::vector<StmtPtr> Content;
	};

	struct ForStmt : Stmt
	{
		ForStmt(size_t line, StmtPtr pre, ExprPtr condition, StmtPtr loop, StmtPtr body);

		StmtPtr Pre;
		ExprPtr Condition;
		StmtPtr Loop;
		StmtPtr Body;
	};

	struct VarStmt : Stmt
	{
		VarStmt(size_t line, const std::string& type, const std::string& name, ExprPtr value);

		std::string Type;
		std::string Name;
		ExprPtr Value;
	};

	struct IncStmt : Stmt
	{
		IncStmt(size_t line, const std::string& filename);

		std::string Filename;
	};

	struct WhileStmt : Stmt
	{
		WhileStmt(size_t line, ExprPtr condition, StmtPtr body);

		ExprPtr Condition;
		StmtPtr Body;
	};

	struct IfStmt : Stmt
	{
		IfStmt(size_t line, ExprPtr condition, StmtPtr _true, StmtPtr _false);

		ExprPtr Condition;
		StmtPtr True;
		StmtPtr False;
	};

	struct ThingStmt : Stmt
	{
		ThingStmt(size_t line, const std::string& name, const std::string& group, const std::map<std::string, std::string>& elements);

		std::string Name;
		std::string Group;
		std::map<std::string, std::string> Elements;
	};

	struct AliasStmt : Stmt
	{
		AliasStmt(size_t line, const std::string& name, const std::string& origin);

		std::string Name;
		std::string Origin;
	};
}
