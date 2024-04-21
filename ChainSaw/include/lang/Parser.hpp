#pragma once

#include <lang/Def.hpp>

#include <functional>
#include <istream>
#include <string>
#include <vector>

namespace csaw::lang
{
    enum TokenType
    {
        TK_EOF,

        TK_IDENTIFIER,

        TK_DEC,
        TK_HEX,
        TK_BIN,

        TK_STRING,
        TK_CHAR,
        TK_OPERATOR,

        TK_COMP_DIR,
    };

    struct Token
    {
        Token(TokenType type, const std::string &value, size_t line);

        Token(TokenType type, int value, size_t line);

        explicit Token(size_t line);

        Token();

        TokenType Type;
        std::string Value;
        size_t Line;
    };

    std::ostream &operator<<(std::ostream &out, const Token &token);

    std::ostream &operator<<(std::ostream &out, const TokenType &type);

    typedef std::function<void(StmtPtr stmt)> ParseCallback;

    class Parser
    {
    public:
        static void
        Parse(std::istream &stream, const ParseCallback &callback, const std::vector<std::string> &includePaths);

    private:
        static int Escape(int c);

    private:
        explicit Parser(std::istream &stream, const ParseCallback &callback,
                        const std::vector<std::string> &includePaths);

        Token &Next();

        [[nodiscard]] bool AtEOF() const;

        [[nodiscard]] bool At(TokenType type) const;

        [[nodiscard]] bool At(const std::string &value) const;

        Token Get();

        Token Expect(TokenType type);

        void Expect(const std::string &value);

        bool NextIfAt(TokenType type);

        bool NextIfAt(const std::string &value);

        void ParseCompileDirective();

        StmtPtr ParseStmt(bool end = true);

        FunStmtPtr ParseFunStmt(bool end);

        RetStmtPtr ParseRetStmt(bool end);

        EnclosedStmtPtr ParseEnclosedStmt();

        ForStmtPtr ParseForStmt();

        VarStmtPtr ParseVarStmt(ExprPtr expr, bool end);

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

        std::istream &m_Stream;
        ParseCallback m_Callback;
        const std::vector<std::string> &m_IncludePaths;
    };
}
