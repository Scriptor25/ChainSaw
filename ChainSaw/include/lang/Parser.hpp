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

    std::ostream &operator<<(std::ostream &out, const TokenType &type);

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

    typedef std::function<void(StmtPtr stmt)> ParseCallback;

    class Parser
    {
    public:
        static void Parse(std::istream &stream,
                          const ParseCallback &callback,
                          const std::vector<std::string> &includePaths);

    private:
        static int Escape(int c);

    private:
        explicit Parser(std::istream &stream,
                        const ParseCallback &callback,
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

        void ParseCompileDirective(); // \x

        TypePtr ParseType();

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

        ExprPtr ParseBinExpr(); // x op y

        ExprPtr ParseLogicExpr(); // x [&, &=, &&, |, |=, ||, ^, ^=] y

        ExprPtr ParseCmpExpr(); // x [=, ==, !=] y

        ExprPtr ParseShiftExpr(); // x [<, >, <=, >=, <<, >>, <<=, >>=] y

        ExprPtr ParseSumExpr(); // x [+, +=, (++), -, -=, (--)] y

        ExprPtr ParseProExpr(); // x [*, *=, /, /=, %, %=] y

        ExprPtr ParseIndexExpr(); // x[y]

        ExprPtr ParseCallExpr(); // x(...)

        ExprPtr ParseMemberExpr(); // x.y

        ExprPtr ParseMemberExpr(ExprPtr expr); // x.y

        ExprPtr ParsePrimExpr(); // x

    private:
        size_t m_Line = 1;
        Token m_Token;

        std::istream &m_Stream;
        ParseCallback m_Callback;
        const std::vector<std::string> &m_IncludePaths;
    };
}
