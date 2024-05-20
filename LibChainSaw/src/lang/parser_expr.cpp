#include <stdexcept>
#include <csaw/Error.hpp>
#include <csaw/lang/Def.hpp>
#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Parser.hpp>

csaw::ExpressionPtr csaw::Parser::ParseExpression()
{
    return ParseSelectExpression();
}

csaw::ExpressionPtr csaw::Parser::ParseSelectExpression()
{
    auto expr = ParseBinaryExpression();

    if (NextIfAt("?"))
    {
        auto line = m_Line;

        auto _true = ParseExpression();
        Expect(":");
        auto _false = ParseExpression();

        expr = std::make_shared<SelectExpression>(m_Data.Filename, line, expr, _true, _false);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseBinaryExpression()
{
    return ParseLogicExpression();
}

csaw::ExpressionPtr csaw::Parser::ParseLogicExpression()
{
    auto expr = ParseCompareExpression();

    while (At("&") || At("|") || At("^"))
    {
        auto line = m_Line;

        std::string op = Get().Value;
        if (At("=") || At(op))
            op += Get().Value;

        ExpressionPtr right;
        if (op == "&=" || op == "|=" || op == "^=")
            right = ParseExpression();
        else
            right = ParseCompareExpression();

        expr = std::make_shared<BinaryExpression>(m_Data.Filename, line, op, expr, right);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseCompareExpression()
{
    auto expr = ParseShiftExpression();

    while (At("=") || At("!"))
    {
        auto line = m_Line;

        std::string op = Get().Value;
        if (At("="))
            op += Get().Value;

        ExpressionPtr right;
        if (op == "=")
            right = ParseExpression();
        else
            right = ParseShiftExpression();

        expr = std::make_shared<BinaryExpression>(m_Data.Filename, line, op, expr, right);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseShiftExpression()
{
    auto expr = ParseSumExpression();

    while (At("<") || At(">"))
    {
        auto line = m_Line;

        std::string op = Get().Value;
        while (At("=") || At(op))
            op += Get().Value;

        ExpressionPtr right;
        if (op == "<<=" || op == ">>=")
            right = ParseExpression();
        else
            right = ParseSumExpression();

        expr = std::make_shared<BinaryExpression>(m_Data.Filename, line, op, expr, right);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseSumExpression()
{
    auto expr = ParseProductExpression();

    while (At("+") || At("-"))
    {
        auto line = m_Line;

        std::string op = Get().Value;
        if (At("=") || At(op))
            op += Get().Value;

        ExpressionPtr right;
        if (op == "+=" || op == "-=")
            right = ParseExpression();
        else if (op == "++" || op == "--")
        {
            expr = std::make_shared<UnaryExpression>(m_Data.Filename, line, op, expr, true);
            break;
        }
        else
            right = ParseProductExpression();

        expr = std::make_shared<BinaryExpression>(m_Data.Filename, line, op, expr, right);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseProductExpression()
{
    auto expr = ParseIndexExpression();

    while (At("*") || At("/") || At("%"))
    {
        auto line = m_Line;

        std::string op = Get().Value;
        if (At("=") || At(op))
            op += Get().Value;

        ExpressionPtr right;
        if (op == "*=" || op == "/=" || op == "%=")
            right = ParseExpression();
        else
            right = ParseIndexExpression();

        expr = std::make_shared<BinaryExpression>(m_Data.Filename, line, op, expr, right);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseIndexExpression()
{
    auto expr = ParseCallExpression();

    while (NextIfAt("["))
    {
        auto line = m_Line;

        auto index = ParseExpression();
        Expect("]");

        expr = std::make_shared<IndexExpression>(m_Data.Filename, line, expr, index);

        if (At("."))
            expr = ParseMemberExpression(expr);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseCallExpression()
{
    auto expr = ParseMemberExpression();

    while (NextIfAt("("))
    {
        auto line = m_Line;

        std::vector<ExpressionPtr> args;
        while (!AtEOF() && !At(")"))
        {
            args.push_back(ParseExpression());
            if (!At(")"))
                Expect(",");
        }
        Expect(")");

        expr = std::make_shared<CallExpression>(m_Data.Filename, line, expr, args);

        if (At("."))
            expr = ParseMemberExpression(expr);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseMemberExpression()
{
    return ParseMemberExpression(ParsePrimaryExpression());
}

csaw::ExpressionPtr csaw::Parser::ParseMemberExpression(ExpressionPtr expr)
{
    while (At(".") || At("!"))
    {
        auto line = m_Line;
        auto deref = Get().Value == "!";
        if (deref && NextIfAt("="))
        {
            const auto right = ParseShiftExpression();
            expr = std::make_shared<BinaryExpression>(m_Data.Filename, line, "!=", expr, right);
        }
        else
        {
            std::string id = Expect(TK_IDENTIFIER | TK_STRING).Value;
            expr = std::make_shared<MemberExpression>(m_Data.Filename, line, expr, id, deref);
        }
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParsePrimaryExpression()
{
    auto line = m_Line;

    if (AtEOF())
        ThrowError(m_Data.Filename, line, true, "Reached end of file");

    if (At(TK_IDENTIFIER)) return std::make_shared<IdentifierExpression>(m_Data.Filename, line, Get().Value);
    if (At(TK_INT_BIN | TK_INT_OCT | TK_INT_DEC | TK_INT_HEX)) return std::make_shared<IntExpression>(m_Data.Filename, line, Get().IntValue());
    if (At(TK_FLOAT)) return std::make_shared<FloatExpression>(m_Data.Filename, line, Get().Value);
    if (At(TK_STRING)) return std::make_shared<StringExpression>(m_Data.Filename, line, Get().Value);
    if (At(TK_CHAR)) return std::make_shared<CharExpression>(m_Data.Filename, line, Get().Value);

    if (At("+") || At("-") || At("!") || At("~"))
    {
        std::string op = Get().Value;
        if ((op == "+" || op == "-") && At(op))
            op += Get().Value;
        return std::make_shared<UnaryExpression>(m_Data.Filename, line, op, ParseIndexExpression(), false);
    }

    if (NextIfAt("("))
    {
        auto expr = ParseExpression();
        Expect(")");
        return expr;
    }

    if (NextIfAt("?"))
    {
        Expect("[");
        const auto type = ParseType();
        Expect("]");
        return std::make_shared<VarArgExpression>(m_Data.Filename, line, type);
    }

    if (NextIfAt("["))
    {
        const auto type = ParseType();
        Expect("]");
        const auto castee = ParseIndexExpression();
        return std::make_shared<CastExpression>(m_Data.Filename, line, type, castee);
    }

    if (NextIfAt("&"))
    {
        const auto value = ParseIndexExpression();
        return std::make_shared<ReferenceExpression>(m_Data.Filename, line, value);
    }

    if (NextIfAt("*"))
    {
        const auto value = ParseIndexExpression();
        return std::make_shared<DereferenceExpression>(m_Data.Filename, line, value);
    }

    if (NextIfAt("$"))
    {
        Expect("(");
        const auto type = ParseType();
        Expect(")");
        return std::make_shared<SizeOfExpression>(m_Data.Filename, line, type);
    }

    const auto [Type, Value, Line] = Get();
    ThrowError(m_Data.Filename, line, true, "Unhandled token type %s value '%s'", TkToString(Type).c_str(), Value.c_str());
    return nullptr;
}
