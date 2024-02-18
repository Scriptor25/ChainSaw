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
	auto line = m_Line;

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
		return std::make_shared<FunStmt>(line, constructor, name, result, params, vararg, callee, EnclosedStmtPtr());
	}

	auto body = ParseEnclosedStmt();
	return std::make_shared<FunStmt>(line, constructor, name, result, params, vararg, callee, body);
}

csaw::lang::RetStmtPtr csaw::lang::Parser::ParseRetStmt(bool end)
{
	auto line = m_Line;

	ExpectAndNext("ret");

	if (NextIfAt(";"))
		return std::make_shared<RetStmt>(line, ExprPtr());

	auto value = ParseExpr();
	if (end) ExpectAndNext(";");

	return std::make_shared<RetStmt>(line, value);
}

csaw::lang::EnclosedStmtPtr csaw::lang::Parser::ParseEnclosedStmt()
{
	auto line = m_Line;

	ExpectAndNext("{");
	std::vector<StmtPtr> content;
	while (!AtEOF() && !At("}"))
		content.push_back(ParseStmt());
	ExpectAndNext("}");

	return std::make_shared<EnclosedStmt>(line, content);
}

csaw::lang::ForStmtPtr csaw::lang::Parser::ParseForStmt()
{
	auto line = m_Line;

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

	return std::make_shared<ForStmt>(line, pre, condition, loop, body);
}

csaw::lang::VarStmtPtr csaw::lang::Parser::ParseVarStmt(ExprPtr expr, bool end)
{
	auto line = m_Line;

	if (auto type = std::dynamic_pointer_cast<IdentExpr>(expr))
	{
		std::string name = ExpectAndNext(TK_IDENTIFIER).Value;

		if (!At("="))
		{
			if (end) ExpectAndNext(";");
			return std::make_shared<VarStmt>(line, type->Id, name, ExprPtr());
		}

		ExpectAndNext("=");
		auto value = ParseExpr();

		if (end) ExpectAndNext(";");
		return std::make_shared<VarStmt>(line, type->Id, name, value);
	}

	return VarStmtPtr();
}

csaw::lang::IncStmtPtr csaw::lang::Parser::ParseIncStmt(bool end)
{
	auto line = m_Line;

	ExpectAndNext("inc");

	std::string filename = ExpectAndNext(TK_STRING).Value;

	if (end) ExpectAndNext(";");
	return std::make_shared<IncStmt>(line, filename);
}

csaw::lang::WhileStmtPtr csaw::lang::Parser::ParseWhileStmt()
{
	auto line = m_Line;

	ExpectAndNext("while");
	ExpectAndNext("(");

	auto condition = ParseExpr();

	ExpectAndNext(")");

	auto body = ParseStmt();

	return std::make_shared<WhileStmt>(line, condition, body);
}

csaw::lang::IfStmtPtr csaw::lang::Parser::ParseIfStmt()
{
	auto line = m_Line;

	ExpectAndNext("if");
	ExpectAndNext("(");

	auto condition = ParseExpr();

	ExpectAndNext(")");

	auto _true = ParseStmt();
	if (!NextIfAt("else"))
		return std::make_shared<IfStmt>(line, condition, _true, StmtPtr());

	auto _false = ParseStmt();
	return std::make_shared<IfStmt>(line, condition, _true, _false);
}

csaw::lang::ThingStmtPtr csaw::lang::Parser::ParseThingStmt(bool end)
{
	auto line = m_Line;

	ExpectAndNext("thing");
	ExpectAndNext(":");
	std::string name = ExpectAndNext(TK_IDENTIFIER).Value;

	std::string group;
	if (NextIfAt(":"))
		group = ExpectAndNext(TK_IDENTIFIER).Value;

	std::map<std::string, std::string> elements;
	if (!At("{"))
	{
		if (end) ExpectAndNext(";");
		return std::make_shared<ThingStmt>(line, name, group, elements);
	}

	ExpectAndNext("{");
	while (!AtEOF() && !At("}"))
	{
		std::string ename = ExpectAndNext(TK_IDENTIFIER).Value;
		ExpectAndNext(":");
		std::string etype = ExpectAndNext(TK_IDENTIFIER).Value;
		elements[ename] = etype;

		if (!At("}"))
			ExpectAndNext(",");
	}
	ExpectAndNext("}");

	return std::make_shared<ThingStmt>(line, name, group, elements);
}

csaw::lang::AliasStmtPtr csaw::lang::Parser::ParseAliasStmt(bool end)
{
	auto line = m_Line;

	ExpectAndNext("alias");
	std::string name = ExpectAndNext(TK_IDENTIFIER).Value;
	ExpectAndNext(":");
	std::string origin = ExpectAndNext(TK_IDENTIFIER).Value;

	if (end) ExpectAndNext(";");
	return std::make_shared<AliasStmt>(line, name, origin);
}

csaw::lang::Stmt::Stmt(size_t line)
	: Line(line)
{
}

csaw::lang::EnclosedStmt::EnclosedStmt(size_t line, const std::vector<StmtPtr>& content)
	: Stmt(line), Content(content)
{
}

csaw::lang::FunStmt::FunStmt(size_t line, bool constructor, const std::string& name, const std::string& result, const std::vector<std::pair<std::string, std::string>>& params, bool vararg, const std::string& callee, EnclosedStmtPtr body)
	: Stmt(line), Constructor(constructor), Name(name), Result(result), Args(params), VarArg(vararg), Callee(callee), Body(body)
{
}

csaw::lang::RetStmt::RetStmt(size_t line, ExprPtr value)
	: Stmt(line), Value(value)
{
}

csaw::lang::ForStmt::ForStmt(size_t line, StmtPtr pre, ExprPtr condition, StmtPtr loop, StmtPtr body)
	: Stmt(line), Pre(pre), Condition(condition), Loop(loop), Body(body)
{
}

csaw::lang::VarStmt::VarStmt(size_t line, const std::string& type, const std::string& name, ExprPtr value)
	: Stmt(line), Type(type), Name(name), Value(value)
{
}

csaw::lang::IncStmt::IncStmt(size_t line, const std::string& filename)
	: Stmt(line), Filename(filename)
{
}

csaw::lang::WhileStmt::WhileStmt(size_t line, ExprPtr condition, StmtPtr body)
	: Stmt(line), Condition(condition), Body(body)
{
}

csaw::lang::IfStmt::IfStmt(size_t line, ExprPtr condition, StmtPtr _true, StmtPtr _false)
	: Stmt(line), Condition(condition), True(_true), False(_false)
{
}

csaw::lang::ThingStmt::ThingStmt(size_t line, const std::string& name, const std::string& group, const std::map<std::string, std::string>& elements)
	: Stmt(line), Name(name), Group(group), Elements(elements)
{
}

csaw::lang::AliasStmt::AliasStmt(size_t line, const std::string& name, const std::string& origin)
	: Stmt(line), Name(name), Origin(origin)
{
}
