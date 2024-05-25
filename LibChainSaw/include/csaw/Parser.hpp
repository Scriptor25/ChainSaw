#pragma once

#include <filesystem>
#include <functional>
#include <vector>
#include <csaw/Def.hpp>

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

    struct Token
    {
        [[nodiscard]] uint64_t IntValue() const;

        SourceLoc Loc;
        TokenType Type;
        std::string Value;
    };

    std::string TkToString(int type);
    std::ostream& operator<<(std::ostream& out, const TokenType& type);
    std::ostream& operator<<(std::ostream& out, const Token& token);

    typedef std::function<void(const StatementPtr& ptr)> ParseCallback;

    struct ParseData
    {
        std::string Filename;
        std::istream& Stream;

        ParseCallback Callback;
        const std::vector<std::string>& IncludePaths;
        std::vector<std::filesystem::path>& Processed;
    };

    class Parser
    {
    public:
        static int Parse(const ParseData& data);

    private:
        explicit Parser(const ParseData& data);

        int Read();
        void PutBack(int c);
        int Escape();
        Token& Next();

        [[nodiscard]] bool AtEOF() const, At(int type) const, At(const std::string& value) const;

        Token Get();
        Token Expect(int type);
        void Expect(const std::string& value);
        bool NextIfAt(const std::string& value);

        void ParseCompileDirective();

        Arg ParseArg();
        TypePtr ParseType(), ParseType(const TypePtr& base);

        StatementPtr ParseStatement(bool end = true);
        DefStatementPtr ParseDefStatement();
        ForStatementPtr ParseForStatement();
        FunctionStatementPtr ParseFunctionStatement();
        IfStatementPtr ParseIfStatement();
        RetStatementPtr ParseRetStatement(bool end);
        ScopeStatementPtr ParseScopeStatement();
        VariableStatementPtr ParseVariableStatement(const ExpressionPtr& expr, bool end);
        WhileStatementPtr ParseWhileStatement();

        ExpressionPtr ParseExpression(),
                      ParseSelectExpression(),
                      ParseBinaryExpression(),
                      ParseLogicExpression(),
                      ParseCompareExpression(),
                      ParseShiftExpression(),
                      ParseSumExpression(),
                      ParseProductExpression(),
                      ParseIndexExpression(),
                      ParseCallExpression(),
                      ParseMemberExpression(),
                      ParseMemberExpression(ExpressionPtr expr),
                      ParsePrimaryExpression();

        ParseData m_Data;

        size_t m_Col = 1;

        SourceLoc m_Loc;
        Token m_Token;
    };
}
