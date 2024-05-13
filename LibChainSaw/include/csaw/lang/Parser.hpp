#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <vector>
#include <csaw/Type.hpp>
#include <csaw/lang/Def.hpp>

namespace csaw
{
    enum TokenType
    {
        TK_EOF = 0b0,

        TK_IDENTIFIER = 0b1,

        TK_INT_BIN = 0b10,
        TK_INT_OCT = 0b100,
        TK_INT_DEC = 0b1000,
        TK_INT_HEX = 0b10000,

        TK_FLOAT = 0b100000,

        TK_STRING = 0b1000000,
        TK_CHAR = 0b10000000,
        TK_OPERATOR = 0b100000000,

        TK_COMPILE_DIRECTIVE = 0b1000000000,
    };

    const char* ToString(int type);
    std::ostream& operator<<(std::ostream& out, const TokenType& type);

    struct Token
    {
        Token(TokenType type, const std::string& value, size_t line);
        Token(TokenType type, int value, size_t line);
        explicit Token(size_t line);
        Token();

        [[nodiscard]] uint32_t IntValue() const;

        TokenType Type;
        std::string Value;
        size_t Line;
    };

    std::ostream& operator<<(std::ostream& out, const Token& token);

    typedef std::function<void(const StatementPtr& ptr)> ParseCallback;

    class Parser
    {
    public:
        static void Parse(const std::string& filename, std::istream& stream, const ParseCallback& callback, const std::vector<std::string>& includePaths);

    private:
        static int Escape(int c);

        Parser(const std::string& filename, std::istream& stream, const ParseCallback& callback, const std::vector<std::string>& includePaths);

        Token& Next();

        [[nodiscard]] bool AtEOF() const;
        [[nodiscard]] bool At(int type) const;
        [[nodiscard]] bool At(const std::string& value) const;

        Token Get();
        Token Expect(int type);
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
        ExpressionPtr ParseLogicExpression(); // x [&, &=, &&, |, |=, ||, ^] y
        ExpressionPtr ParseCompareExpression(); // x [=, ==, !=] y
        ExpressionPtr ParseShiftExpression(); // x [<, >, <=, >=, <<, >>, <<=, >>=] y
        ExpressionPtr ParseSumExpression(); // x [+, +=, (++), -, -=, (--)] y
        ExpressionPtr ParseProductExpression(); // x [*, *=, /, /=, %, %=] y
        ExpressionPtr ParseIndexExpression(); // x[y]
        ExpressionPtr ParseCallExpression(); // x(...)
        ExpressionPtr ParseMemberExpression(); // x.y
        ExpressionPtr ParseMemberExpression(ExpressionPtr expr); // x.y
        ExpressionPtr ParsePrimaryExpression(); // x

        size_t m_Line = 1;
        Token m_Token;

        std::string m_Filename;
        std::istream& m_Stream;
        ParseCallback m_Callback;
        const std::vector<std::string>& m_IncludePaths;
    };
}
