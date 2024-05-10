#include <csaw/CSaw.hpp>
#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Parser.hpp>
#include <csaw/lang/Stmt.hpp>

csaw::StatementPtr csaw::Parser::ParseStatement(const bool end)
{
    while (At(TK_COMPILE_DIRECTIVE)) ParseCompileDirective();

    if (At("{")) return ParseScopeStatement();
    if (At("for")) return ParseForStatement();
    if (At("@") || At("$")) return ParseFunctionStatement();
    if (At("if")) return ParseIfStatement();
    if (At("ret")) return ParseRetStatement(end);
    if (At("def")) return ParseDefStatement();
    if (At("while")) return ParseWhileStatement();

    auto expr = ParseExpression();
    if (auto stmt = ParseVariableStatement(expr, end))
        return stmt;

    if (end) Expect(";");

    return expr;
}

csaw::ScopeStatementPtr csaw::Parser::ParseScopeStatement()
{
    auto line = m_Line;

    Expect("{");
    std::vector<StatementPtr> content;
    while (!AtEOF() && !At("}"))
        content.push_back(ParseStatement());
    Expect("}");

    return std::make_shared<ScopeStatement>(line, content);
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

    return std::make_shared<ForStatement>(line, pre, condition, loop, body);
}

csaw::FunctionStatementPtr csaw::Parser::ParseFunctionStatement()
{
    auto line = m_Line;

    std::string name;
    TypePtr callee_type = nullptr;
    TypePtr result_type = nullptr;
    std::vector<std::pair<std::string, TypePtr>> args;
    bool is_constructor = false;
    bool is_vararg = false;
    StatementPtr body = nullptr;
    std::vector<ExpressionPtr> alt_body;

    is_constructor = NextIfAt("$");
    if (!is_constructor)
        Expect("@");

    if (NextIfAt("("))
    {
        do { name += Get().Value; }
        while (!At(")") && !AtEOF());
        Expect(")");
    }
    else name = Expect(TK_IDENTIFIER).Value;

    if (NextIfAt(":"))
        if (!At(":"))
            callee_type = ParseType();

    if (NextIfAt("("))
    {
        while (!At(")") && !AtEOF())
        {
            if (NextIfAt("?"))
            {
                is_vararg = true;
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

    if (NextIfAt(":"))
        result_type = ParseType();

    if (NextIfAt("="))
    {
        body = ParseExpression();
        Expect(";");
        return std::make_shared<FunctionStatement>(line, name, callee_type, result_type, args, is_constructor, is_vararg, body);
    }

    if (NextIfAt("["))
    {
        do
        {
            alt_body.push_back(ParseExpression());
            if (!At("]"))
                Expect(",");
        }
        while (!At("]") && !AtEOF());
        Expect("]");
        CSAW_MESSAGE(true, "Feature is not yet implemented");
    }

    if (!At("{"))
    {
        Expect(";");
        return std::make_shared<FunctionStatement>(line, name, callee_type, result_type, args, is_constructor, is_vararg, body);
    }

    body = ParseScopeStatement();
    return std::make_shared<FunctionStatement>(line, name, callee_type, result_type, args, is_constructor, is_vararg, body);
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
        return std::make_shared<IfStatement>(line, condition, _true, StatementPtr());

    auto _false = ParseStatement();
    return std::make_shared<IfStatement>(line, condition, _true, _false);
}

csaw::RetStatementPtr csaw::Parser::ParseRetStatement(const bool end)
{
    auto line = m_Line;

    Expect("ret");

    if (NextIfAt(";"))
        return std::make_shared<RetStatement>(line, ExpressionPtr());

    auto value = ParseExpression();
    if (end) Expect(";");

    return std::make_shared<RetStatement>(line, value);
}

csaw::DefStatementPtr csaw::Parser::ParseDefStatement()
{
    auto line = m_Line;

    Expect("def");
    std::string name = Expect(TK_IDENTIFIER).Value;

    if (NextIfAt(";"))
        return std::make_shared<DefStatement>(line, name);

    if (!NextIfAt("{"))
    {
        Expect("=");
        const auto type = ParseType();
        Expect(";");
        return std::make_shared<DefStatement>(line, name, type);
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

    StructType::Get(name, elements);

    return std::make_shared<DefStatement>(line, name, elements);
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
            return std::make_shared<VariableStatement>(line, name->Id, type, nullptr);
        }

        Expect("=");
        auto value = ParseExpression();

        if (end) Expect(";");
        return std::make_shared<VariableStatement>(line, name->Id, type, value);
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

    return std::make_shared<WhileStatement>(line, condition, body);
}
