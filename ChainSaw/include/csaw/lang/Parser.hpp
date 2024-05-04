#pragma once

#include <functional>
#include <string>
#include <vector>
#include <csaw/Type.hpp>
#include <csaw/lang/Def.hpp>

namespace csaw
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

        TK_COMPILE_DIRECTIVE,
    };

    const char* ToString(TokenType type);
    std::ostream& operator<<(std::ostream& out, const TokenType& type);

    struct Token
    {
        Token(TokenType type, const std::string& value, size_t line);
        Token(TokenType type, int value, size_t line);
        explicit Token(size_t line);
        Token();

        TokenType Type;
        std::string Value;
        size_t Line;
    };

    std::ostream& operator<<(std::ostream& out, const Token& token);

    typedef std::function<void(const StatementPtr& ptr)> ParseCallback;

    class Parser
    {
    public:
        static void Parse(std::istream& stream, const ParseCallback& callback, const std::vector<std::string>& includePaths);

    private:
        static int Escape(int c);

    private:
        explicit Parser(std::istream& stream, const ParseCallback& callback, const std::vector<std::string>& includePaths);

        Token& Next();

        [[nodiscard]] bool AtEOF() const;
        [[nodiscard]] bool At(TokenType type) const;
        [[nodiscard]] bool At(const std::string& value) const;

        Token Get();
        Token Expect(TokenType type);
        void Expect(const std::string& value);
        bool NextIfAt(const std::string& value);

        void ParseCompileDirective(); // \x

        TypePtr ParseType();
        TypePtr ParseType(const TypePtr& base);

        StatementPtr ParseStatement(bool end = true);
        FunctionStatementPtr ParseFunctionStatement();
        RetStatementPtr ParseRetStatement(bool end);
        ScopeStatementPtr ParseScopeStatement();
        ForStatementPtr ParseForStatement();
        VariableStatementPtr ParseVariableStatement(const ExpressionPtr& expr, bool end);
        WhileStatementPtr ParseWhileStatement();
        IfStatementPtr ParseIfStatement();
        DefStatementPtr ParseDefStatement();

        ExpressionPtr ParseExpression();
        ExpressionPtr ParseSelectExpression(); // x ? y : z
        ExpressionPtr ParseBinaryExpression(); // x op y
        ExpressionPtr ParseLogicExpression(); // x [&, &=, &&, |, |=, ||, ^, ^=] y
        ExpressionPtr ParseCompareExpression(); // x [=, ==, !=] y
        ExpressionPtr ParseShiftExpression(); // x [<, >, <=, >=, <<, >>, <<=, >>=] y
        ExpressionPtr ParseSumExpression(); // x [+, +=, (++), -, -=, (--)] y
        ExpressionPtr ParseProductExpression(); // x [*, *=, /, /=, %, %=] y
        ExpressionPtr ParseIndexExpression(); // x[y]
        ExpressionPtr ParseCallExpression(); // x(...)
        ExpressionPtr ParseMemberExpression(); // x.y
        ExpressionPtr ParseMemberExpression(ExpressionPtr expr); // x.y
        ExpressionPtr ParsePrimaryExpression(); // x

    private:
        size_t m_Line = 1;
        Token m_Token;

        std::istream& m_Stream;
        ParseCallback m_Callback;
        const std::vector<std::string>& m_IncludePaths;
    };
}
