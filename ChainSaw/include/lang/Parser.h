#pragma once

#include <lang/ASTDef.h>
#include <runtime/Runtime.h>

#include <istream>
#include <string>
#include <vector>

namespace csaw::lang
{
	enum TokenType
	{
		TK_,
		TK_EOF,

		TK_IDENTIFIER,

		TK_DEC,
		TK_HEX,
		TK_BIN,

		TK_STRING,
		TK_CHAR,
		TK_OPERATOR,
	};

	struct Token
	{
		Token(const TokenType type, const std::string& value, const size_t line);
		Token(const TokenType type, const int value, const size_t line);
		Token(const size_t line);
		Token();

		TokenType Type;
		std::string Value;
		size_t Line;
	};

	std::ostream& operator<<(std::ostream& out, const Token& token);
	std::ostream& operator<<(std::ostream& out, const TokenType& type);

	class Parser
	{
	public:
		static void Parse(std::istream& stream, csaw::runtime::EnvironmentPtr env);

	private:
		Parser(std::istream& stream);
		~Parser();

		int Read();
		void Mark();
		void Reset();
		int Escape(int c);

		Token& Next();

		bool AtEOF() const;
		bool At(const TokenType type) const;
		bool At(const std::string& value) const;

		Token& Expect(const TokenType type);
		Token& Expect(const std::string& value);

		Token GetAndNext();
		Token ExpectAndNext(const TokenType type);
		Token ExpectAndNext(const std::string& value);

		bool NextIfAt(const TokenType type);
		bool NextIfAt(const std::string& value);

		StmtPtr ParseStmt(bool end = true);
		FunStmtPtr ParseFunStmt(bool end);
		RetStmtPtr ParseRetStmt(bool end);
		EnclosedStmtPtr ParseEnclosedStmt();
		ForStmtPtr ParseForStmt();
		VarStmtPtr ParseVarStmt(ExprPtr expr, bool end);
		IncStmtPtr ParseIncStmt(bool end);
		WhileStmtPtr ParseWhileStmt();
		IfStmtPtr ParseIfStmt();
		ThingStmtPtr ParseThingStmt(bool end);
		AliasStmtPtr ParseAliasStmt(bool end);

		ExprPtr ParseExpr();
		ExprPtr ParseSelExpr(); // x ? y : z
		ExprPtr ParseBinExpr();
		ExprPtr ParseLogicExpr(); // &, &=, &&, |, |=, ||, ^, ^=
		ExprPtr ParseCmpExpr(); // =, ==, !=
		ExprPtr ParseShiftExpr(); // <, >, <=, >=, <<, >>, <<=, >>=
		ExprPtr ParseSumExpr(); // +, +=, (++), -, -=, (--)
		ExprPtr ParseProExpr(); // *, *=, /, /=, %, %=
		ExprPtr ParseIndexExpr();
		ExprPtr ParseCallExpr();
		ExprPtr ParseMemberExpr();
		ExprPtr ParseMemberExpr(ExprPtr expr);
		ExprPtr ParsePrimExpr();

	private:
		size_t m_Line = 1;
		Token m_Token;

		std::istream& m_Stream;
		bool m_Marked = false;
		std::vector<int> m_Buffered;
	};
}
