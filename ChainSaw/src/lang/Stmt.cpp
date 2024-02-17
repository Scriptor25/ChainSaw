#include <lang/Parser.h>
#include <lang/Stmt.h>
#include <lang/Expr.h>

#include <cassert>

csaw::lang::StmtPtr csaw::lang::Parser::ParseStmt(bool end)
{
	if (At("@") || At("$")) return ParseFunStmt(end);
	if (At("ret")) return ParseRetStmt(end);
	if (At("{")) return ParseEnclosedStmt();
	if (At("for")) return ParseForStmt();
	if (At("inc")) return ParseIncStmt(end);
	if (At("while")) return ParseWhileStmt();
	if (At("if")) return ParseIfStmt();
	if (At("thing")) return ParseThingStmt(end);
	if (At("alias")) return ParseAliasStmt(end);

	auto expr = ParseExpr();
	if (auto vstmt = ParseVarStmt(expr, end))
		return vstmt;

	if (end) ExpectAndNext(";");

	return expr;
}

csaw::lang::FunStmtPtr csaw::lang::Parser::ParseFunStmt(bool end)
{
	bool constructor = false;
	std::string name;
	std::string result;
	std::vector<std::pair<std::string, std::string>> params;
	bool vararg = false;
	std::string callee;

	constructor = NextIfAt("$");
	if (!constructor) ExpectAndNext("@");

	// Either (name) or name
	if (NextIfAt("("))
	{
		do
		{
			name += GetAndNext().Value;
		} while (!AtEOF() && !At(")"));
		ExpectAndNext(")");
	}
	else name = ExpectAndNext(TK_IDENTIFIER).Value;

	if (constructor) result = name;

	// Optional: return result
	if (NextIfAt(":"))
	{
		assert(!constructor);
		result = ExpectAndNext(TK_IDENTIFIER).Value;
	}

	// Optional: parameters
	if (NextIfAt("("))
	{
		while (!AtEOF() && !At(")"))
		{
			std::string pname = ExpectAndNext(TK_IDENTIFIER).Value;
			ExpectAndNext(":");
			std::string ptype = ExpectAndNext(TK_IDENTIFIER).Value;
			params.push_back({ pname, ptype });

			if (!At(")"))
				ExpectAndNext(",");
		}
		ExpectAndNext(")");
	}

	// Optional: vararg flag
	vararg = NextIfAt("?");

	if (NextIfAt("-"))
	{
		ExpectAndNext(">");
		callee = ExpectAndNext(TK_IDENTIFIER).Value;
	}

	if (!At("{"))
	{
		if (end) ExpectAndNext(";");
		return FunStmt::Ptr(constructor, name, result, params, vararg, callee);
	}

	auto body = ParseEnclosedStmt();
	return FunStmt::Ptr(constructor, name, result, params, vararg, callee, body);
}

csaw::lang::RetStmtPtr csaw::lang::Parser::ParseRetStmt(bool end)
{
	ExpectAndNext("ret");

	if (NextIfAt(";"))
		return RetStmt::Ptr();

	auto value = ParseExpr();
	if (end) ExpectAndNext(";");

	return RetStmt::Ptr(value);
}

csaw::lang::EnclosedStmtPtr csaw::lang::Parser::ParseEnclosedStmt()
{
	ExpectAndNext("{");
	std::vector<StmtPtr> content;
	while (!AtEOF() && !At("}"))
		content.push_back(ParseStmt());
	ExpectAndNext("}");

	return EnclosedStmt::Ptr(content);
}

csaw::lang::ForStmtPtr csaw::lang::Parser::ParseForStmt()
{
	StmtPtr pre;
	ExprPtr condition;
	StmtPtr loop;

	ExpectAndNext("for");
	ExpectAndNext("(");
	if (!NextIfAt(";"))
		pre = ParseStmt();
	if (!NextIfAt(";"))
	{
		condition = ParseExpr();
		ExpectAndNext(";");
	}
	if (!At(")"))
		loop = ParseStmt(false);
	ExpectAndNext(")");

	auto body = ParseStmt();

	return ForStmt::Ptr(pre, condition, loop, body);
}

csaw::lang::VarStmtPtr csaw::lang::Parser::ParseVarStmt(ExprPtr expr, bool end)
{
	if (auto type = std::dynamic_pointer_cast<IdentExpr>(expr))
	{
		std::string name = ExpectAndNext(TK_IDENTIFIER).Value;

		if (!At("="))
		{
			if (end) ExpectAndNext(";");
			return VarStmt::Ptr(type->Id, name);
		}

		ExpectAndNext("=");
		auto value = ParseExpr();

		if (end) ExpectAndNext(";");
		return VarStmt::Ptr(type->Id, name, value);
	}

	return VarStmtPtr();
}

csaw::lang::IncStmtPtr csaw::lang::Parser::ParseIncStmt(bool end)
{
	ExpectAndNext("inc");

	std::string filename = ExpectAndNext(TK_STRING).Value;

	if (end) ExpectAndNext(";");
	return IncStmt::Ptr(filename);
}

csaw::lang::WhileStmtPtr csaw::lang::Parser::ParseWhileStmt()
{
	ExpectAndNext("while");
	ExpectAndNext("(");

	auto condition = ParseExpr();

	ExpectAndNext(")");

	auto body = ParseStmt();

	return WhileStmt::Ptr(condition, body);
}

csaw::lang::IfStmtPtr csaw::lang::Parser::ParseIfStmt()
{
	ExpectAndNext("if");
	ExpectAndNext("(");

	auto condition = ParseExpr();

	ExpectAndNext(")");

	auto _true = ParseStmt();
	if (!NextIfAt("else"))
		return IfStmt::Ptr(condition, _true);

	auto _false = ParseStmt();
	return IfStmt::Ptr(condition, _true, _false);
}

csaw::lang::ThingStmtPtr csaw::lang::Parser::ParseThingStmt(bool end)
{
	ExpectAndNext("thing");
	ExpectAndNext(":");
	std::string name = ExpectAndNext(TK_IDENTIFIER).Value;

	std::string group;
	if (NextIfAt(":"))
		group = ExpectAndNext(TK_IDENTIFIER).Value;

	if (!At("{"))
	{
		if (end) ExpectAndNext(";");
		return ThingStmt::Ptr(name, group);
	}

	std::vector<std::pair<std::string, std::string>> elements;
	ExpectAndNext("{");
	while (!AtEOF() && !At("}"))
	{
		std::string ename = ExpectAndNext(TK_IDENTIFIER).Value;
		ExpectAndNext(":");
		std::string etype = ExpectAndNext(TK_IDENTIFIER).Value;
		elements.push_back({ ename, etype });

		if (!At("}"))
			ExpectAndNext(",");
	}
	ExpectAndNext("}");

	return ThingStmt::Ptr(name, group, elements);
}

csaw::lang::AliasStmtPtr csaw::lang::Parser::ParseAliasStmt(bool end)
{
	ExpectAndNext("alias");
	std::string name = ExpectAndNext(TK_IDENTIFIER).Value;
	ExpectAndNext(":");
	std::string origin = ExpectAndNext(TK_IDENTIFIER).Value;

	if (end) ExpectAndNext(";");
	return AliasStmt::Ptr(name, origin);
}

csaw::lang::EnclosedStmtPtr csaw::lang::EnclosedStmt::Ptr(const std::vector<StmtPtr>& content)
{
	return std::make_shared<EnclosedStmt>(content);
}

csaw::lang::EnclosedStmt::EnclosedStmt(const std::vector<StmtPtr>& content)
	: Content(content)
{
}

csaw::lang::FunStmtPtr csaw::lang::FunStmt::Ptr(const bool constructor, const std::string& name, const std::string& result, const std::vector<std::pair<std::string, std::string>>& params, const bool vararg, const std::string& callee, const EnclosedStmtPtr body)
{
	return std::make_shared<FunStmt>(constructor, name, result, params, vararg, callee, body);
}

csaw::lang::FunStmtPtr csaw::lang::FunStmt::Ptr(const bool constructor, const std::string& name, const std::string& result, const std::vector<std::pair<std::string, std::string>>& params, const bool vararg, const std::string& callee)
{
	return std::make_shared<FunStmt>(constructor, name, result, params, vararg, callee, EnclosedStmtPtr());
}

csaw::lang::FunStmt::FunStmt(const bool constructor, const std::string& name, const std::string& result, const std::vector<std::pair<std::string, std::string>>& params, const bool vararg, const std::string& callee, const EnclosedStmtPtr body)
	: Constructor(constructor), Name(name), Result(result), Params(params), VarArg(vararg), Callee(callee), Body(body)
{
}

csaw::lang::RetStmtPtr csaw::lang::RetStmt::Ptr(const ExprPtr value)
{
	return std::make_shared<RetStmt>(value);
}

csaw::lang::RetStmtPtr csaw::lang::RetStmt::Ptr()
{
	return std::make_shared<RetStmt>(ExprPtr());
}

csaw::lang::RetStmt::RetStmt(const ExprPtr value)
	: Value(value)
{
}

csaw::lang::ForStmtPtr csaw::lang::ForStmt::Ptr(const StmtPtr pre, const ExprPtr condition, const StmtPtr loop, const StmtPtr body)
{
	return std::make_shared<ForStmt>(pre, condition, loop, body);
}

csaw::lang::ForStmt::ForStmt(const StmtPtr pre, const ExprPtr condition, const StmtPtr loop, const StmtPtr body)
	: Pre(pre), Condition(condition), Loop(loop), Body(body)
{
}

csaw::lang::VarStmtPtr csaw::lang::VarStmt::Ptr(const std::string& type, const std::string& name, const ExprPtr value)
{
	return std::make_shared<VarStmt>(type, name, value);
}

csaw::lang::VarStmtPtr csaw::lang::VarStmt::Ptr(const std::string& type, const std::string& name)
{
	return std::make_shared<VarStmt>(type, name, ExprPtr());
}

csaw::lang::VarStmt::VarStmt(const std::string& type, const std::string& name, const ExprPtr value)
	: Type(type), Name(name), Value(value)
{
}

csaw::lang::IncStmtPtr csaw::lang::IncStmt::Ptr(const std::string& filename)
{
	return std::make_shared<IncStmt>(filename);
}

csaw::lang::IncStmt::IncStmt(const std::string& filename)
	: Filename(filename)
{
}

csaw::lang::WhileStmtPtr csaw::lang::WhileStmt::Ptr(const ExprPtr condition, const StmtPtr body)
{
	return std::make_shared<WhileStmt>(condition, body);
}

csaw::lang::WhileStmt::WhileStmt(const ExprPtr condition, const StmtPtr body)
	: Condition(condition), Body(body)
{
}

csaw::lang::IfStmtPtr csaw::lang::IfStmt::Ptr(const ExprPtr condition, const StmtPtr _true, const StmtPtr _false)
{
	return std::make_shared<IfStmt>(condition, _true, _false);
}

csaw::lang::IfStmtPtr csaw::lang::IfStmt::Ptr(const ExprPtr condition, const StmtPtr _true)
{
	return std::make_shared<IfStmt>(condition, _true, StmtPtr());
}

csaw::lang::IfStmt::IfStmt(const ExprPtr condition, const StmtPtr _true, const StmtPtr _false)
	: Condition(condition), True(_true), False(_false)
{
}

csaw::lang::ThingStmtPtr csaw::lang::ThingStmt::Ptr(const std::string& name, const std::string& group, const std::vector<std::pair<std::string, std::string>>& elements)
{
	return std::make_shared<ThingStmt>(name, group, elements);
}

csaw::lang::ThingStmtPtr csaw::lang::ThingStmt::Ptr(const std::string& name, const std::string& group)
{
	return std::make_shared<ThingStmt>(name, group, std::vector<std::pair<std::string, std::string>>());
}

csaw::lang::ThingStmt::ThingStmt(const std::string& name, const std::string& group, const std::vector<std::pair<std::string, std::string>>& elements)
	: Name(name), Group(group), Elements(elements)
{
}

csaw::lang::AliasStmtPtr csaw::lang::AliasStmt::Ptr(const std::string& name, const std::string& origin)
{
	return std::make_shared<AliasStmt>(name, origin);
}

csaw::lang::AliasStmt::AliasStmt(const std::string& name, const std::string& origin)
	: Name(name), Origin(origin)
{
}
