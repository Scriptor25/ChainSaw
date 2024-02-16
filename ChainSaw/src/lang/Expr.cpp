#include <lang/Parser.h>
#include <lang/Expr.h>

#include <cassert>

csaw::lang::ExprPtr csaw::lang::Parser::ParseExpr()
{
	return ParseSelExpr();
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseSelExpr()
{
	auto expr = ParseBinExpr();

	if (NextIfAt("?"))
	{
		auto _true = ParseExpr();
		ExpectAndNext(":");
		auto _false = ParseExpr();

		expr = SelExpr::Ptr(expr, _true, _false);
	}

	return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseBinExpr()
{
	return ParseLogicExpr();
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseLogicExpr()
{
	auto expr = ParseCmpExpr();

	while (At("&") || At("|") || At("^"))
	{
		std::string op = GetAndNext().Value;
		if (At("=") || At(op))
			op += GetAndNext().Value;

		assert(op == "&" || op == "&=" || op == "&&" || op == "|" || op == "|=" || op == "||" || op == "^" || op == "^=");

		ExprPtr right;
		if (op == "&=" || op == "|=" || op == "^=")
			right = ParseExpr();
		else
			right = ParseCmpExpr();

		expr = BinExpr::Ptr(op, expr, right);
	}

	return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseCmpExpr()
{
	auto expr = ParseShiftExpr();

	while (At("=") || At("!"))
	{
		std::string op = GetAndNext().Value;
		if (At("="))
			op += GetAndNext().Value;

		assert(op == "=" || op == "==" || op == "!=");

		ExprPtr right;
		if (op == "=")
			right = ParseExpr();
		else
			right = ParseShiftExpr();

		expr = BinExpr::Ptr(op, expr, right);
	}

	return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseShiftExpr()
{
	auto expr = ParseSumExpr();

	while (At("<") || At(">"))
	{
		std::string op = GetAndNext().Value;
		while (At("=") || At(op))
			op += GetAndNext().Value;

		assert(op == "<" || op == ">" || op == "<=" || op == ">=" || op == "<<" || op == ">>" || op == "<<=" || op == ">>=");

		ExprPtr right;
		if (op == "<<=" || op == ">>=")
			right = ParseExpr();
		else
			right = ParseSumExpr();

		expr = BinExpr::Ptr(op, expr, right);
	}

	return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseSumExpr()
{
	auto expr = ParseProExpr();

	while (At("+") || At("-"))
	{
		std::string op = GetAndNext().Value;
		if (At("=") || At(op))
			op += GetAndNext().Value;

		assert(op == "+" || op == "+=" || op == "++" || op == "-" || op == "-=" || op == "--");

		ExprPtr right;
		if (op == "+=" || op == "-=")
			right = ParseExpr();
		else if (op == "++" || op == "--")
		{
			expr = UnExpr::Ptr(op, expr, true);
			break;
		}
		else
			right = ParseProExpr();

		expr = BinExpr::Ptr(op, expr, right);
	}

	return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseProExpr()
{
	auto expr = ParseIndexExpr();

	while (At("*") || At("/") || At("%"))
	{
		std::string op = GetAndNext().Value;
		if (At("=") || At(op))
			op += GetAndNext().Value;

		assert(op == "*" || op == "*=" || op == "/" || op == "/=" || op == "%" || op == "%=");

		ExprPtr right;
		if (op == "*=" || op == "/=" || op == "%=")
			right = ParseExpr();
		else
			right = ParseIndexExpr();

		expr = BinExpr::Ptr(op, expr, right);
	}

	return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseIndexExpr()
{
	auto expr = ParseCallExpr();

	while (NextIfAt("["))
	{
		auto index = ParseExpr();
		ExpectAndNext("]");

		expr = IndexExpr::Ptr(expr, index);
	}

	return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseCallExpr()
{
	auto expr = ParseMemberExpr();

	if (NextIfAt("("))
	{
		std::vector<ExprPtr> args;
		while (!AtEOF() && !At(")"))
		{
			args.push_back(ParseExpr());
			if (!At(")"))
				ExpectAndNext(",");
		}
		ExpectAndNext(")");

		expr = CallExpr::Ptr(expr, args);
	}

	return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseMemberExpr()
{
	auto expr = ParsePrimExpr();

	while (NextIfAt("."))
	{
		std::string id = ExpectAndNext(TK_IDENTIFIER).Value;
		expr = MemberExpr::Ptr(expr, id);
	}

	return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParsePrimExpr()
{
	assert(!AtEOF());

	if (At(TK_DEC))
		return NumExpr::Ptr(GetAndNext().Value, 10);
	if (At(TK_HEX))
		return NumExpr::Ptr(GetAndNext().Value, 16);
	if (At(TK_BIN))
		return NumExpr::Ptr(GetAndNext().Value, 2);

	if (At(TK_CHAR))
		return ChrExpr::Ptr(GetAndNext().Value);

	if (At(TK_STRING))
		return StrExpr::Ptr(GetAndNext().Value);

	if (At(TK_IDENTIFIER))
		return IdentExpr::Ptr(GetAndNext().Value);

	if (At("++") || At("--") || At("-") || At("!") || At("~"))
	{
		std::string op = GetAndNext().Value;
		return UnExpr::Ptr(op, ParseExpr());
	}

	if (NextIfAt("("))
	{
		auto expr = ParseExpr();
		ExpectAndNext(")");
		return expr;
	}

	if (NextIfAt("?"))
	{
		std::string type = ExpectAndNext(TK_IDENTIFIER).Value;
		ExpectAndNext("[");
		auto index = ParseExpr();
		ExpectAndNext("]");
		return VarArgExpr::Ptr(type, index);
	}

	throw;
}

csaw::lang::NumExprPtr csaw::lang::NumExpr::Ptr(const std::string& value, const int radix)
{
	double v;
	switch (radix)
	{
	case 10:
		v = std::stod(value);
		break;
	case 2:
		v = std::stol(value, nullptr, 2);
		break;
	case 16:
		v = std::stol(value, nullptr, 16);
		break;
	default:
		v = 0.0;
		break;
	}

	return std::make_shared<NumExpr>(v);
}

csaw::lang::NumExpr::NumExpr(const double value)
	: Value(value)
{
}

csaw::lang::ChrExprPtr csaw::lang::ChrExpr::Ptr(const std::string& value)
{
	assert(value.length() == 1);
	return std::make_shared<ChrExpr>(value[0]);
}

csaw::lang::ChrExpr::ChrExpr(const char value)
	: Value(value)
{
}

csaw::lang::StrExprPtr csaw::lang::StrExpr::Ptr(const std::string& value)
{
	return std::make_shared<StrExpr>(value);
}

csaw::lang::StrExpr::StrExpr(const std::string& value)
	: Value(value)
{
}

csaw::lang::CallExprPtr csaw::lang::CallExpr::Ptr(const ExprPtr callee, const std::vector<ExprPtr>& args)
{
	return std::make_shared<CallExpr>(callee, args);
}

csaw::lang::CallExpr::CallExpr(const ExprPtr callee, const std::vector<ExprPtr>& args)
	: Callee(callee), Args(args)
{
}

csaw::lang::IdentExprPtr csaw::lang::IdentExpr::Ptr(const std::string& id)
{
	return std::make_shared<IdentExpr>(id);
}

csaw::lang::IdentExpr::IdentExpr(const std::string& id)
	: Id(id)
{
}

csaw::lang::BinExprPtr csaw::lang::BinExpr::Ptr(const std::string& op, const ExprPtr left, const ExprPtr right)
{
	return std::make_shared<BinExpr>(op, left, right);
}

csaw::lang::BinExpr::BinExpr(const std::string& op, const ExprPtr left, const ExprPtr right)
	: Operator(op), Left(left), Right(right)
{
}

csaw::lang::UnExprPtr csaw::lang::UnExpr::Ptr(const std::string& op, const ExprPtr value, const bool rightop)
{
	return std::make_shared<UnExpr>(op, value, rightop);
}

csaw::lang::UnExpr::UnExpr(const std::string& op, const ExprPtr value, const bool rightop)
	: Operator(op), Value(value), RightOp(rightop)
{
}

csaw::lang::IndexExprPtr csaw::lang::IndexExpr::Ptr(const ExprPtr array, const ExprPtr index)
{
	return std::make_shared<IndexExpr>(array, index);
}

csaw::lang::IndexExpr::IndexExpr(const ExprPtr array, const ExprPtr index)
	: Array(array), Index(index)
{
}

csaw::lang::MemberExprPtr csaw::lang::MemberExpr::Ptr(const ExprPtr object, const std::string& member)
{
	return std::make_shared<MemberExpr>(object, member);
}

csaw::lang::MemberExpr::MemberExpr(const ExprPtr object, const std::string& member)
	: Object(object), Member(member)
{
}

csaw::lang::VarArgExprPtr csaw::lang::VarArgExpr::Ptr(const std::string& type, const ExprPtr index)
{
	return std::make_shared<VarArgExpr>(type, index);
}

csaw::lang::VarArgExpr::VarArgExpr(const std::string& type, const ExprPtr index)
	: Type(type), Index(index)
{
}

csaw::lang::SelExprPtr csaw::lang::SelExpr::Ptr(const ExprPtr condition, const ExprPtr _true, const ExprPtr _false)
{
	return std::make_shared<SelExpr>(condition, _true, _false);
}

csaw::lang::SelExpr::SelExpr(const ExprPtr condition, const ExprPtr _true, const ExprPtr _false)
	: Condition(condition), True(_true), False(_false)
{
}
