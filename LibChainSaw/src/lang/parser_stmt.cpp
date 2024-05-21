#include <csaw/Expr.hpp>
#include <csaw/Parser.hpp>
#include <csaw/Stmt.hpp>
#include <csaw/Type.hpp>

csaw::StatementPtr csaw::Parser::ParseStatement(const bool end)
{
    if (At("def")) return ParseDefStatement();
    if (At("for")) return ParseForStatement();
    if (At("@")) return ParseFunctionStatement();
    if (At("if")) return ParseIfStatement();
    if (At("ret")) return ParseRetStatement(end);
    if (At("{")) return ParseScopeStatement();
    if (At("while")) return ParseWhileStatement();

    auto expr = ParseExpression();
    if (auto stmt = ParseVariableStatement(expr, end))
        return stmt;

    if (end) Expect(";");

    return expr;
}

csaw::DefStatementPtr csaw::Parser::ParseDefStatement()
{
    auto loc = m_Token.Loc;

    Expect("def");
    std::string name = Expect(TK_IDENTIFIER).Value;

    if (NextIfAt(";"))
    {
        StructType::Create(name, {});
        return std::make_shared<DefStatement>(loc, name);
    }

    if (!NextIfAt("{"))
    {
        Expect("=");
        const auto type = ParseType();
        Expect(";");
        return std::make_shared<DefStatement>(loc, name, type);
    }

    std::vector<Arg> elements;
    while (!AtEOF() && !At("}"))
    {
        auto e_name = Expect(TK_IDENTIFIER).Value;
        Expect(":");
        const auto e_type = ParseType();
        elements.emplace_back(e_name, e_type);

        if (!At("}"))
            Expect(",");
    }
    Expect("}");

    StructType::Create(name, elements);

    return std::make_shared<DefStatement>(loc, name, elements);
}

csaw::ForStatementPtr csaw::Parser::ParseForStatement()
{
    auto loc = m_Token.Loc;

    StatementPtr pre;
    ExpressionPtr condition;
    StatementPtr loop;

    Expect("for");
    Expect("(");
    if (!NextIfAt(";"))
        pre = ParseStatement();
    if (!NextIfAt(";"))
    {
        condition = ParseExpression();
        Expect(";");
    }
    if (!At(")"))
        loop = ParseStatement(false);
    Expect(")");

    auto body = ParseStatement();

    return std::make_shared<ForStatement>(loc, pre, condition, loop, body);
}

csaw::FunctionStatementPtr csaw::Parser::ParseFunctionStatement()
{
    const auto loc = m_Token.Loc;

    Expect("@");

    std::vector<std::string> mods;
    if (NextIfAt("["))
    {
        while (!At("]") && !AtEOF())
        {
            const auto mod = Expect(TK_IDENTIFIER).Value;
            mods.push_back(mod);
            if (!At("]"))
                Expect(",");
        }
        Expect("]");
    }

    const auto name = Expect(TK_IDENTIFIER | TK_STRING).Value;

    TypePtr parent;
    if (NextIfAt(":"))
        if (!At(":"))
            parent = ParseType();

    std::vector<Arg> args;
    bool is_varargs = false;

    if (NextIfAt("("))
    {
        while (!At(")") && !AtEOF())
        {
            if (NextIfAt("?"))
            {
                is_varargs = true;
                break;
            }

            args.push_back(ParseArg());

            if (!At(")"))
                Expect(",");
        }
        Expect(")");
    }

    TypePtr result;
    if (NextIfAt(":"))
        result = ParseType();

    if (NextIfAt("="))
    {
        const auto body = ParseExpression();
        Expect(";");
        return std::make_shared<FunctionStatement>(loc, name, parent, result, mods, args, is_varargs, body);
    }

    if (!At("{"))
    {
        Expect(";");
        return std::make_shared<FunctionStatement>(loc, name, parent, result, mods, args, is_varargs, StatementPtr());
    }

    const auto body = ParseScopeStatement();
    return std::make_shared<FunctionStatement>(loc, name, parent, result, mods, args, is_varargs, body);
}

csaw::IfStatementPtr csaw::Parser::ParseIfStatement()
{
    auto loc = m_Token.Loc;

    Expect("if");
    Expect("(");

    auto condition = ParseExpression();

    Expect(")");

    auto _true = ParseStatement();
    if (!NextIfAt("else"))
        return std::make_shared<IfStatement>(loc, condition, _true, StatementPtr());

    auto _false = ParseStatement();
    return std::make_shared<IfStatement>(loc, condition, _true, _false);
}

csaw::RetStatementPtr csaw::Parser::ParseRetStatement(const bool end)
{
    auto loc = m_Token.Loc;

    Expect("ret");

    if (NextIfAt(";"))
        return std::make_shared<RetStatement>(loc, ExpressionPtr());

    auto value = ParseExpression();
    if (end) Expect(";");

    return std::make_shared<RetStatement>(loc, value);
}

csaw::ScopeStatementPtr csaw::Parser::ParseScopeStatement()
{
    auto loc = m_Token.Loc;

    Expect("{");
    std::vector<StatementPtr> content;
    while (!AtEOF() && !At("}"))
        content.push_back(ParseStatement());
    Expect("}");

    return std::make_shared<ScopeStatement>(loc, content);
}

csaw::VariableStatementPtr csaw::Parser::ParseVariableStatement(const ExpressionPtr& expr, const bool end)
{
    auto loc = m_Token.Loc;

    if (const auto name = std::dynamic_pointer_cast<IdentifierExpression>(expr))
    {
        if (!NextIfAt(":"))
            return nullptr;

        const auto type = ParseType();

        if (!At("="))
        {
            if (end) Expect(";");
            return std::make_shared<VariableStatement>(loc, name->Id, type, nullptr);
        }

        Expect("=");
        auto value = ParseExpression();

        if (end) Expect(";");
        return std::make_shared<VariableStatement>(loc, name->Id, type, value);
    }

    return nullptr;
}

csaw::WhileStatementPtr csaw::Parser::ParseWhileStatement()
{
    auto loc = m_Token.Loc;

    Expect("while");
    Expect("(");
    auto condition = ParseExpression();
    Expect(")");

    auto body = ParseStatement();
    return std::make_shared<WhileStatement>(loc, condition, body);
}
