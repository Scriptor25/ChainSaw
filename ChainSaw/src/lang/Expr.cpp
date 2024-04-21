#include <lang/Assert.hpp>
#include <lang/Parser.hpp>
#include <lang/Expr.hpp>

csaw::lang::ExprPtr csaw::lang::Parser::ParseExpr()
{
    return ParseSelExpr();
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseSelExpr()
{
    auto expr = ParseBinExpr();

    if (NextIfAt("?"))
    {
        auto line = m_Line;

        auto _true = ParseExpr();
        Expect(":");
        auto _false = ParseExpr();

        expr = std::make_shared<SelExpr>(line, expr, _true, _false);
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
        auto line = m_Line;

        std::string op = Get().Value;
        if (At("=") || At(op))
            op += Get().Value;

        CHAINSAW_ASSERT(op == "&"
                        || op == "&="
                        || op == "&&"
                        || op == "|"
                        || op == "|="
                        || op == "||"
                        || op == "^"
                        || op == "^=",
                        "Unexpected operator");

        ExprPtr right;
        if (op == "&=" || op == "|=" || op == "^=")
            right = ParseExpr();
        else
            right = ParseCmpExpr();

        expr = std::make_shared<BinExpr>(line, op, expr, right);
    }

    return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseCmpExpr()
{
    auto expr = ParseShiftExpr();

    while (At("=") || At("!"))
    {
        auto line = m_Line;

        std::string op = Get().Value;
        if (At("="))
            op += Get().Value;

        CHAINSAW_ASSERT(op == "=" || op == "==" || op == "!=", "Unexpected operator");

        ExprPtr right;
        if (op == "=")
            right = ParseExpr();
        else
            right = ParseShiftExpr();

        expr = std::make_shared<BinExpr>(line, op, expr, right);
    }

    return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseShiftExpr()
{
    auto expr = ParseSumExpr();

    while (At("<") || At(">"))
    {
        auto line = m_Line;

        std::string op = Get().Value;
        while (At("=") || At(op))
            op += Get().Value;

        CHAINSAW_ASSERT(op == "<"
                        || op == ">"
                        || op == "<="
                        || op == ">="
                        || op == "<<"
                        || op == ">>"
                        || op == "<<="
                        || op == ">>=",
                        "Unexpected operator");

        ExprPtr right;
        if (op == "<<=" || op == ">>=")
            right = ParseExpr();
        else
            right = ParseSumExpr();

        expr = std::make_shared<BinExpr>(line, op, expr, right);
    }

    return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseSumExpr()
{
    auto expr = ParseProExpr();

    while (At("+") || At("-"))
    {
        auto line = m_Line;

        std::string op = Get().Value;
        if (At("=") || At(op))
            op += Get().Value;

        CHAINSAW_ASSERT(op == "+"
                        || op == "+="
                        || op == "++"
                        || op == "-"
                        || op == "-="
                        || op == "--",
                        "Unexpected operator");

        ExprPtr right;
        if (op == "+=" || op == "-=")
            right = ParseExpr();
        else if (op == "++" || op == "--")
        {
            expr = std::make_shared<UnExpr>(line, op, expr, true);
            break;
        }
        else
            right = ParseProExpr();

        expr = std::make_shared<BinExpr>(line, op, expr, right);
    }

    return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseProExpr()
{
    auto expr = ParseIndexExpr();

    while (At("*") || At("/") || At("%"))
    {
        auto line = m_Line;

        std::string op = Get().Value;
        if (At("=") || At(op))
            op += Get().Value;

        CHAINSAW_ASSERT(op == "*"
                        || op == "*="
                        || op == "/"
                        || op == "/="
                        || op == "%"
                        || op == "%=",
                        "Unexpected operator");

        ExprPtr right;
        if (op == "*=" || op == "/=" || op == "%=")
            right = ParseExpr();
        else
            right = ParseIndexExpr();

        expr = std::make_shared<BinExpr>(line, op, expr, right);
    }

    return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseIndexExpr()
{
    auto expr = ParseCallExpr();

    while (NextIfAt("["))
    {
        auto line = m_Line;

        auto index = ParseExpr();
        Expect("]");

        expr = std::make_shared<IndexExpr>(line, expr, index);

        if (At("."))
            expr = ParseMemberExpr(expr);
    }

    return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseCallExpr()
{
    auto expr = ParseMemberExpr();

    while (NextIfAt("("))
    {
        auto line = m_Line;

        std::vector<ExprPtr> args;
        while (!AtEOF() && !At(")"))
        {
            args.push_back(ParseExpr());
            if (!At(")"))
                Expect(",");
        }
        Expect(")");

        expr = std::make_shared<CallExpr>(line, expr, args);

        if (At("."))
            expr = ParseMemberExpr(expr);
    }

    return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseMemberExpr()
{
    return ParseMemberExpr(ParsePrimExpr());
}

csaw::lang::ExprPtr csaw::lang::Parser::ParseMemberExpr(ExprPtr expr)
{
    while (NextIfAt("."))
    {
        auto line = m_Line;

        std::string id = Expect(TK_IDENTIFIER).Value;
        expr = std::make_shared<MemberExpr>(line, expr, id);
    }

    return expr;
}

csaw::lang::ExprPtr csaw::lang::Parser::ParsePrimExpr()
{
    auto line = m_Line;

    CHAINSAW_ASSERT(!AtEOF(), "Reached end of file");

    if (At(TK_DEC))
        return std::make_shared<NumExpr>(line, Get().Value, 10);
    if (At(TK_HEX))
        return std::make_shared<NumExpr>(line, Get().Value, 16);
    if (At(TK_BIN))
        return std::make_shared<NumExpr>(line, Get().Value, 2);

    if (At(TK_CHAR))
        return std::make_shared<ChrExpr>(line, Get().Value);

    if (At(TK_STRING))
        return std::make_shared<StrExpr>(line, Get().Value);

    if (At(TK_IDENTIFIER))
        return std::make_shared<IdentExpr>(line, Get().Value);

    if (At("+") || At("-") || At("!") || At("~"))
    {
        std::string op = Get().Value;
        if ((op == "+" || op == "-") && At(op))
            op += Get().Value;
        return std::make_shared<UnExpr>(line, op, ParseExpr(), false);
    }

    if (NextIfAt("("))
    {
        auto expr = ParseExpr();
        Expect(")");
        return expr;
    }

    if (NextIfAt("?"))
    {
        std::string type = Expect(TK_IDENTIFIER).Value;
        Expect("[");
        auto index = ParseExpr();
        Expect("]");
        return std::make_shared<VarArgExpr>(line, type, index);
    }

    throw;
}

csaw::lang::Expr::Expr(size_t line)
        : Stmt(line)
{
}

csaw::lang::NumExpr::NumExpr(size_t line, double value)
        : Expr(line), Value(value)
{
}

csaw::lang::NumExpr::NumExpr(size_t line, const std::string &value, int base)
        : Expr(line)
{
    switch (base)
    {
        case 2:
        case 16: Value = double(std::stoll(value, nullptr, base));
            break;

        case 10: Value = std::stod(value);
            break;

        default: throw;
    }
}

csaw::lang::ChrExpr::ChrExpr(size_t line, char value)
        : Expr(line), Value(value)
{
}

csaw::lang::ChrExpr::ChrExpr(size_t line, const std::string &value)
        : Expr(line)
{
    CHAINSAW_ASSERT(value.length() == 1, "Char needs to have a length of 1");
    Value = value[0];
}

csaw::lang::StrExpr::StrExpr(size_t line, const std::string &value)
        : Expr(line), Value(value)
{
}

csaw::lang::CallExpr::CallExpr(size_t line, ExprPtr callee, const std::vector<ExprPtr> &args)
        : Expr(line), Callee(callee), Args(args)
{
}

csaw::lang::IdentExpr::IdentExpr(size_t line, const std::string &id)
        : Expr(line), Id(id)
{
}

csaw::lang::BinExpr::BinExpr(size_t line, const std::string &op, ExprPtr left, ExprPtr right)
        : Expr(line), Operator(op), Left(left), Right(right)
{
}

csaw::lang::UnExpr::UnExpr(size_t line, const std::string &op, ExprPtr value, bool rightop)
        : Expr(line), Operator(op), Value(value), RightOp(rightop)
{
}

csaw::lang::IndexExpr::IndexExpr(size_t line, ExprPtr array, ExprPtr index)
        : Expr(line), Array(array), Index(index)
{
}

csaw::lang::MemberExpr::MemberExpr(size_t line, ExprPtr object, const std::string &member)
        : Expr(line), Object(object), Member(member)
{
}

csaw::lang::VarArgExpr::VarArgExpr(size_t line, const std::string &type, ExprPtr index)
        : Expr(line), Type(type), Index(index)
{
}

csaw::lang::SelExpr::SelExpr(size_t line, ExprPtr condition, ExprPtr _true, ExprPtr _false)
        : Expr(line), Condition(condition), True(_true), False(_false)
{
}
