#include <lang/ASTDef.h>
#include <lang/Stmt.h>
#include <lang/Expr.h>

#include <string>

static size_t depth = 0;
static bool end = true;

static std::string spaces()
{
	std::string spaces;
	for (size_t i = 0; i < depth; i++)
		spaces += "    ";
	return spaces;
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const FunStmt& stmt)
{
	out << '@' << stmt.Name;
	if (!stmt.Result.empty())
		out << ": " << stmt.Result;

	if (!stmt.Params.empty())
	{
		out << " (";
		for (size_t i = 0; i < stmt.Params.size(); i++)
		{
			if (i > 0)
				out << ", ";
			out << stmt.Params[i].first << ": " << stmt.Params[i].second;
		}
		out << ')';
	}

	if (stmt.VarArg)
		out << " ?";

	if (!stmt.Body)
		return out << ';';
	return out << ' ' << *stmt.Body;
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const RetStmt& stmt)
{
	out << "ret";
	if (stmt.Value)
		out << ' ' << stmt.Value;

	if (end)
		return out << ';';
	return out;
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const EnclosedStmt& stmt)
{
	depth++;
	auto sp = spaces();
	out << '{' << '\n';
	for (auto& s : stmt.Content)
		out << sp << s << '\n';
	depth--;
	return out << '}';
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const ForStmt& stmt)
{
	out << "for (";
	if (stmt.Pre) out << stmt.Pre;
	else out << ';';
	out << ' ';
	if (stmt.Condition) out << stmt.Condition;
	out << "; ";
	end = false;
	if (stmt.Loop) out << stmt.Loop;
	end = true;
	return out << ") " << stmt.Body;
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const VarStmt& stmt)
{
	out << stmt.Type << ' ' << stmt.Name;
	if (!stmt.Value)
		if (end) return out << ';';
		else return out;
	out << " = " << stmt.Value;
	if (end) return out << ';';
	return out;
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const StmtPtr ptr)
{
	if (!ptr)
		return out << "nullptr";

	if (auto stmt = std::dynamic_pointer_cast<FunStmt>(ptr))
		return out << *stmt;
	if (auto stmt = std::dynamic_pointer_cast<RetStmt>(ptr))
		return out << *stmt;
	if (auto stmt = std::dynamic_pointer_cast<EnclosedStmt>(ptr))
		return out << *stmt;
	if (auto stmt = std::dynamic_pointer_cast<ForStmt>(ptr))
		return out << *stmt;
	if (auto stmt = std::dynamic_pointer_cast<VarStmt>(ptr))
		return out << *stmt;

	if (auto expr = std::dynamic_pointer_cast<Expr>(ptr))
	{
		out << expr;
		if (end) return out << ';';
		return out;
	}

	throw;
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const CallExpr& expr)
{
	out << expr.Callee << '(';
	for (size_t i = 0; i < expr.Args.size(); i++)
	{
		if (i > 0)
			out << ", ";
		out << expr.Args[i];
	}
	return out << ')';
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const NumExpr& expr)
{
	return out << expr.Value;
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const ChrExpr& expr)
{
	return out << '\'' << expr.Value << '\'';
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const StrExpr& expr)
{
	std::string value;
	for (auto c : expr.Value)
	{
		switch (c)
		{
		case '\a':
			value += "\\a";
			break;
		case '\b':
			value += "\\b";
			break;
		case '\f':
			value += "\\f";
			break;
		case '\n':
			value += "\\n";
			break;
		case '\r':
			value += "\\r";
			break;
		case '\t':
			value += "\\t";
			break;
		case '\v':
			value += "\\v";
			break;

		default:
			value += c;
			break;
		}
	}

	return out << '"' << value << '"';
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const IdentExpr& expr)
{
	return out << expr.Id;
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const BinExpr& expr)
{
	return out << expr.Left << ' ' << expr.Operator << ' ' << expr.Right;
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const UnExpr& expr)
{
	if (!expr.RightOp) out << expr.Operator;
	out << expr.Value;
	if (expr.RightOp) out << expr.Operator;
	return out;
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const IndexExpr& expr)
{
	return out << expr.Array << '[' << expr.Index << ']';
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const MemberExpr& expr)
{
	return out << expr.Object << '.' << expr.Member;
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const VarArgExpr& expr)
{
	return out << '?' << expr.Type << '[' << expr.Index << ']';
}

std::ostream& csaw::lang::operator<<(std::ostream& out, const ExprPtr ptr)
{
	if (auto expr = std::dynamic_pointer_cast<CallExpr>(ptr))
		return out << *expr;
	if (auto expr = std::dynamic_pointer_cast<NumExpr>(ptr))
		return out << *expr;
	if (auto expr = std::dynamic_pointer_cast<ChrExpr>(ptr))
		return out << *expr;
	if (auto expr = std::dynamic_pointer_cast<StrExpr>(ptr))
		return out << *expr;
	if (auto expr = std::dynamic_pointer_cast<IdentExpr>(ptr))
		return out << *expr;
	if (auto expr = std::dynamic_pointer_cast<BinExpr>(ptr))
		return out << *expr;
	if (auto expr = std::dynamic_pointer_cast<UnExpr>(ptr))
		return out << *expr;
	if (auto expr = std::dynamic_pointer_cast<IndexExpr>(ptr))
		return out << *expr;
	if (auto expr = std::dynamic_pointer_cast<MemberExpr>(ptr))
		return out << *expr;
	if (auto expr = std::dynamic_pointer_cast<VarArgExpr>(ptr))
		return out << *expr;

	throw;
}
