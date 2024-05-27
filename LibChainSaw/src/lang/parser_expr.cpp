#include <csaw/Def.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Parser.hpp>

csaw::ExpressionPtr csaw::Parser::ParseExpression()
{
    return ParseSelectExpression();
}

csaw::ExpressionPtr csaw::Parser::ParseSelectExpression()
{
    auto loc = m_Token.Loc;
    auto expr = ParseBinaryExpression();

    if (NextIfAt("?"))
    {
        auto _true = ParseExpression();
        Expect(":");
        auto _false = ParseExpression();

        expr = std::make_shared<SelectExpression>(loc, expr, _true, _false);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseBinaryExpression()
{
    return ParseLogicExpression();
}

csaw::ExpressionPtr csaw::Parser::ParseLogicExpression()
{
    auto loc = m_Token.Loc;
    auto expr = ParseCompareExpression();

    while (At("&") || At("|") || At("^"))
    {
        std::string op = Get().Value;
        if (At("=") || At(op))
            op += Get().Value;

        ExpressionPtr right;
        if (op == "&=" || op == "|=" || op == "^=")
            right = ParseExpression();
        else
            right = ParseCompareExpression();

        expr = std::make_shared<BinaryExpression>(loc, op, expr, right);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseCompareExpression()
{
    auto loc = m_Token.Loc;
    auto expr = ParseShiftExpression();

    while (At("=") || At("!"))
    {
        std::string op = Get().Value;
        if (At("="))
            op += Get().Value;

        ExpressionPtr right;
        if (op == "=")
            right = ParseExpression();
        else
            right = ParseShiftExpression();

        expr = std::make_shared<BinaryExpression>(loc, op, expr, right);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseShiftExpression()
{
    auto loc = m_Token.Loc;
    auto expr = ParseSumExpression();

    while (At("<") || At(">"))
    {
        std::string op = Get().Value;
        while (At("=") || At(op))
            op += Get().Value;

        ExpressionPtr right;
        if (op == "<<=" || op == ">>=")
            right = ParseExpression();
        else
            right = ParseSumExpression();

        expr = std::make_shared<BinaryExpression>(loc, op, expr, right);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseSumExpression()
{
    auto loc = m_Token.Loc;
    auto expr = ParseProductExpression();

    while (At("+") || At("-"))
    {
        std::string op = Get().Value;
        if (At("=") || At(op))
            op += Get().Value;

        ExpressionPtr right;
        if (op == "+=" || op == "-=")
            right = ParseExpression();
        else if (op == "++" || op == "--")
        {
            expr = std::make_shared<UnaryExpression>(loc, op, expr, true);
            break;
        }
        else
            right = ParseProductExpression();

        expr = std::make_shared<BinaryExpression>(loc, op, expr, right);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseProductExpression()
{
    auto loc = m_Token.Loc;
    auto expr = ParseIndexExpression();

    while (At("*") || At("/") || At("%"))
    {
        std::string op = Get().Value;
        if (At("=") || At(op))
            op += Get().Value;

        ExpressionPtr right;
        if (op == "*=" || op == "/=" || op == "%=")
            right = ParseExpression();
        else
            right = ParseIndexExpression();

        expr = std::make_shared<BinaryExpression>(loc, op, expr, right);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseIndexExpression()
{
    auto loc = m_Token.Loc;
    auto expr = ParseCallExpression();

    while (NextIfAt("["))
    {
        auto index = ParseExpression();
        Expect("]");

        expr = std::make_shared<IndexExpression>(loc, expr, index);

        if (At(".") || At("!"))
            expr = ParseMemberExpression(expr);
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParseCallExpression()
{
    auto loc = m_Token.Loc;
    auto expr = ParseMemberExpression();

    while (At("(") || At("$"))
    {
        std::vector<TypePtr> temp_args;
        if (NextIfAt("$"))
        {
            Expect("<");
            while (!AtEOF() && !At(">"))
            {
                const auto type = ParseType();
                temp_args.push_back(type);

                if (!At(">"))
                    Expect(",");
            }
            Expect(">");
        }

        Expect("(");

        std::vector<ExpressionPtr> args;
        while (!AtEOF() && !At(")"))
        {
            args.push_back(ParseExpression());
            if (!At(")"))
                Expect(",");
        }
        Expect(")");

        expr = std::make_shared<CallExpression>(loc, expr, temp_args, args);

        if (At(".") || At("!"))
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
        auto loc = m_Token.Loc;

        auto deref = Get().Value == "!";

        if (deref && NextIfAt("="))
        {
            const auto right = ParseShiftExpression();
            expr = std::make_shared<BinaryExpression>(loc, "!=", expr, right);
        }
        else
        {
            std::string id = Expect(TK_IDENTIFIER | TK_STRING).Value;
            expr = std::make_shared<MemberExpression>(loc, expr, id, deref);
        }
    }

    return expr;
}

csaw::ExpressionPtr csaw::Parser::ParsePrimaryExpression()
{
    auto loc = m_Token.Loc;

    if (AtEOF())
        ThrowError(m_Token.Loc, true, "Reached end of file");

    if (At(TK_IDENTIFIER)) return std::make_shared<IdentifierExpression>(loc, Get().Value);
    if (At(TK_INT_BIN | TK_INT_OCT | TK_INT_DEC | TK_INT_HEX)) return std::make_shared<IntExpression>(loc, Get().IntValue());
    if (At(TK_FLOAT)) return std::make_shared<FloatExpression>(loc, Get().Value);
    if (At(TK_STRING)) return std::make_shared<StringExpression>(loc, Get().Value);
    if (At(TK_CHAR)) return std::make_shared<CharExpression>(loc, Get().Value);

    if (At("+") || At("-") || At("!") || At("~"))
    {
        std::string op = Get().Value;
        if ((op == "+" || op == "-") && At(op))
            op += Get().Value;
        return std::make_shared<UnaryExpression>(loc, op, ParseIndexExpression(), false);
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
        return std::make_shared<VarArgExpression>(loc, type);
    }

    if (NextIfAt("["))
    {
        const auto type = ParseType();
        Expect("]");
        const auto castee = ParseIndexExpression();
        return std::make_shared<CastExpression>(loc, type, castee);
    }

    if (NextIfAt("&"))
    {
        const auto value = ParseIndexExpression();
        return std::make_shared<ReferenceExpression>(loc, value);
    }

    if (NextIfAt("*"))
    {
        const auto value = ParseIndexExpression();
        return std::make_shared<DereferenceExpression>(loc, value);
    }

    if (NextIfAt("$"))
    {
        Expect("(");
        const auto type = ParseType();
        Expect(")");
        return std::make_shared<SizeOfExpression>(loc, type);
    }

    const auto [Loc, Type, Value] = Get();
    ThrowError(m_Token.Loc, true, "Unhandled token type %s value '%s'", TkToString(Type).c_str(), Value.c_str());
    return nullptr;
}
