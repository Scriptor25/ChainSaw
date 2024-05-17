#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Parser.hpp>
#include <csaw/lang/Stmt.hpp>

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
    auto line = m_Line;

    Expect("def");
    std::string name = Expect(TK_IDENTIFIER).Value;

    if (NextIfAt(";"))
    {
        StructType::Create(name, {});
        return std::make_shared<DefStatement>(m_Data.Filename, line, name);
    }

    if (!NextIfAt("{"))
    {
        Expect("=");
        const auto type = ParseType();
        Expect(";");
        return std::make_shared<DefStatement>(m_Data.Filename, line, name, type);
    }

    std::vector<std::pair<std::string, TypePtr>> elements;
    while (!AtEOF() && !At("}"))
    {
        auto eName = Expect(TK_IDENTIFIER).Value;
        Expect(":");
        const auto eType = ParseType();
        elements.emplace_back(eName, eType);

        if (!At("}"))
            Expect(",");
    }
    Expect("}");

    StructType::Create(name, elements);

    return std::make_shared<DefStatement>(m_Data.Filename, line, name, elements);
}

csaw::ForStatementPtr csaw::Parser::ParseForStatement()
{
    auto line = m_Line;

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

    return std::make_shared<ForStatement>(m_Data.Filename, line, pre, condition, loop, body);
}

csaw::FunctionStatementPtr csaw::Parser::ParseFunctionStatement()
{
    const auto line = m_Line;

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

    std::vector<std::pair<std::string, TypePtr>> args;
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

            auto aName = Expect(TK_IDENTIFIER).Value;
            Expect(":");
            auto aType = ParseType();
            args.emplace_back(aName, aType);
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
        return std::make_shared<FunctionStatement>(m_Data.Filename, line, name, parent, result, mods, args, is_varargs, body);
    }

    if (!At("{"))
    {
        Expect(";");
        return std::make_shared<FunctionStatement>(m_Data.Filename, line, name, parent, result, mods, args, is_varargs, StatementPtr());
    }

    const auto body = ParseScopeStatement();
    return std::make_shared<FunctionStatement>(m_Data.Filename, line, name, parent, result, mods, args, is_varargs, body);
}

csaw::IfStatementPtr csaw::Parser::ParseIfStatement()
{
    auto line = m_Line;

    Expect("if");
    Expect("(");

    auto condition = ParseExpression();

    Expect(")");

    auto _true = ParseStatement();
    if (!NextIfAt("else"))
        return std::make_shared<IfStatement>(m_Data.Filename, line, condition, _true, StatementPtr());

    auto _false = ParseStatement();
    return std::make_shared<IfStatement>(m_Data.Filename, line, condition, _true, _false);
}

csaw::RetStatementPtr csaw::Parser::ParseRetStatement(const bool end)
{
    auto line = m_Line;

    Expect("ret");

    if (NextIfAt(";"))
        return std::make_shared<RetStatement>(m_Data.Filename, line, ExpressionPtr());

    auto value = ParseExpression();
    if (end) Expect(";");

    return std::make_shared<RetStatement>(m_Data.Filename, line, value);
}

csaw::ScopeStatementPtr csaw::Parser::ParseScopeStatement()
{
    auto line = m_Line;

    Expect("{");
    std::vector<StatementPtr> content;
    while (!AtEOF() && !At("}"))
        content.push_back(ParseStatement());
    Expect("}");

    return std::make_shared<ScopeStatement>(m_Data.Filename, line, content);
}

csaw::VariableStatementPtr csaw::Parser::ParseVariableStatement(const ExpressionPtr& expr, const bool end)
{
    auto line = m_Line;

    if (const auto name = std::dynamic_pointer_cast<IdentifierExpression>(expr))
    {
        if (!NextIfAt(":"))
            return nullptr;

        auto type = ParseType();

        if (!At("="))
        {
            if (end) Expect(";");
            return std::make_shared<VariableStatement>(m_Data.Filename, line, name->Id, type, nullptr);
        }

        Expect("=");
        auto value = ParseExpression();

        if (end) Expect(";");
        return std::make_shared<VariableStatement>(m_Data.Filename, line, name->Id, type, value);
    }

    return nullptr;
}

csaw::WhileStatementPtr csaw::Parser::ParseWhileStatement()
{
    auto line = m_Line;

    Expect("while");
    Expect("(");
    auto condition = ParseExpression();
    Expect(")");

    auto body = ParseStatement();
    return std::make_shared<WhileStatement>(m_Data.Filename, line, condition, body);
}
