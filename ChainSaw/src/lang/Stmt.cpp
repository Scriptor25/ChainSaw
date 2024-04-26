#include <csaw/lang/Assert.hpp>
#include <csaw/lang/Parser.hpp>
#include <csaw/lang/Stmt.hpp>
#include <csaw/lang/Expr.hpp>

csaw::StmtPtr csaw::Parser::ParseStmt(bool end)
{
    while (At(TK_COMP_DIR)) ParseCompileDirective();

    if (At("@") || At("$")) return ParseFunStmt(end);
    if (At("ret")) return ParseRetStmt(end);
    if (At("{")) return ParseEnclosedStmt();
    if (At("for")) return ParseForStmt();
    if (At("while")) return ParseWhileStmt();
    if (At("if")) return ParseIfStmt();
    if (At("thing")) return ParseThingStmt(end);
    if (At("alias")) return ParseAliasStmt(end);

    auto expr = ParseExpr();
    if (auto stmt = ParseVarStmt(expr, end))
        return stmt;

    if (end) Expect(";");

    return expr;
}

// @name
// @name::result
// @name(args...):result
// @name:callee
// @name:callee(args...)
// @name:callee:result
// @name:callee(args...):result
csaw::FunStmtPtr csaw::Parser::ParseFunStmt(bool end)
{
    auto line = m_Line;

    bool constructor;
    std::string name;
    TypePtr callee;
    std::vector<std::pair<std::string, TypePtr>> args;
    std::string vararg;
    TypePtr result;
    EnclosedStmtPtr body;

    constructor = NextIfAt("$");
    if (!constructor)
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
            callee = ParseType();

    if (NextIfAt("("))
    {
        while (!At(")") && !AtEOF())
        {
            auto aName = Expect(TK_IDENTIFIER).Value;
            if (NextIfAt("?"))
            {
                vararg = aName;
                break;
            }
            Expect(":");
            auto aType = ParseType();
            args.push_back({aName, aType});
            if (!At(")"))
                Expect(",");
        }
        Expect(")");
    }

    if (NextIfAt(":"))
        result = ParseType();

    if (!At("{"))
    {
        if (end) Expect(";");
        return std::make_shared<FunStmt>(line, constructor, name, callee, args, vararg, result, body);
    }

    body = ParseEnclosedStmt();
    return std::make_shared<FunStmt>(line, constructor, name, callee, args, vararg, result, body);
}

csaw::RetStmtPtr csaw::Parser::ParseRetStmt(bool end)
{
    auto line = m_Line;

    Expect("ret");

    if (NextIfAt(";"))
        return std::make_shared<RetStmt>(line, ExprPtr());

    auto value = ParseExpr();
    if (end) Expect(";");

    return std::make_shared<RetStmt>(line, value);
}

csaw::EnclosedStmtPtr csaw::Parser::ParseEnclosedStmt()
{
    auto line = m_Line;

    Expect("{");
    std::vector<StmtPtr> content;
    while (!AtEOF() && !At("}"))
        content.push_back(ParseStmt());
    Expect("}");

    return std::make_shared<EnclosedStmt>(line, content);
}

csaw::ForStmtPtr csaw::Parser::ParseForStmt()
{
    auto line = m_Line;

    StmtPtr pre;
    ExprPtr condition;
    StmtPtr loop;

    Expect("for");
    Expect("(");
    if (!NextIfAt(";"))
        pre = ParseStmt();
    if (!NextIfAt(";"))
    {
        condition = ParseExpr();
        Expect(";");
    }
    if (!At(")"))
        loop = ParseStmt(false);
    Expect(")");

    auto body = ParseStmt();

    return std::make_shared<ForStmt>(line, pre, condition, loop, body);
}

csaw::VarStmtPtr csaw::Parser::ParseVarStmt(ExprPtr expr, bool end)
{
    auto line = m_Line;

    if (auto name = std::dynamic_pointer_cast<IdentExpr>(expr))
    {
        if (!NextIfAt(":"))
            return nullptr;

        auto type = ParseType();

        if (!At("="))
        {
            if (end) Expect(";");
            return std::make_shared<VarStmt>(line, name->Id, type, nullptr);
        }

        Expect("=");
        auto value = ParseExpr();

        if (end) Expect(";");
        return std::make_shared<VarStmt>(line, name->Id, type, value);
    }

    return nullptr;
}

csaw::WhileStmtPtr csaw::Parser::ParseWhileStmt()
{
    auto line = m_Line;

    Expect("while");
    Expect("(");

    auto condition = ParseExpr();

    Expect(")");

    auto body = ParseStmt();

    return std::make_shared<WhileStmt>(line, condition, body);
}

csaw::IfStmtPtr csaw::Parser::ParseIfStmt()
{
    auto line = m_Line;

    Expect("if");
    Expect("(");

    auto condition = ParseExpr();

    Expect(")");

    auto _true = ParseStmt();
    if (!NextIfAt("else"))
        return std::make_shared<IfStmt>(line, condition, _true, StmtPtr());

    auto _false = ParseStmt();
    return std::make_shared<IfStmt>(line, condition, _true, _false);
}

csaw::ThingStmtPtr csaw::Parser::ParseThingStmt(bool end)
{
    auto line = m_Line;

    Expect("thing");
    Expect(":");
    std::string name = Expect(TK_IDENTIFIER).Value;

    std::string group;
    if (NextIfAt(":"))
        group = Expect(TK_IDENTIFIER).Value;

    std::map<std::string, TypePtr> elements;
    if (!At("{"))
    {
        if (end) Expect(";");
        return std::make_shared<ThingStmt>(line, name, group, elements);
    }

    Expect("{");
    while (!AtEOF() && !At("}"))
    {
        auto eName = Expect(TK_IDENTIFIER).Value;
        Expect(":");
        auto eType = ParseType();
        elements[eName] = eType;

        if (!At("}"))
            Expect(",");
    }
    Expect("}");

    return std::make_shared<ThingStmt>(line, name, group, elements);
}

csaw::AliasStmtPtr csaw::Parser::ParseAliasStmt(bool end)
{
    auto line = m_Line;

    Expect("alias");
    auto name = Expect(TK_IDENTIFIER).Value;
    Expect(":");
    auto origin = ParseType();

    if (end) Expect(";");
    return std::make_shared<AliasStmt>(line, name, origin);
}

csaw::Stmt::Stmt(size_t line)
        : Line(line)
{
}

csaw::EnclosedStmt::EnclosedStmt(size_t line, const std::vector<StmtPtr> &content)
        : Stmt(line), Content(content)
{
}

csaw::FunStmt::FunStmt(size_t line,
                       bool constructor,
                       const std::string &name,
                       const TypePtr callee,
                       const std::vector<std::pair<std::string, TypePtr>> &args,
                       const std::string &vararg,
                       const TypePtr result,
                       const EnclosedStmtPtr body)
        : Stmt(line),
          Constructor(constructor),
          Name(name),
          Callee(callee),
          Args(args),
          VarArg(vararg),
          Result(result),
          Body(body)
{
}

csaw::RetStmt::RetStmt(size_t line, ExprPtr value)
        : Stmt(line), Value(value)
{
}

csaw::ForStmt::ForStmt(size_t line, StmtPtr pre, ExprPtr condition, StmtPtr loop, StmtPtr body)
        : Stmt(line), Pre(pre), Condition(condition), Loop(loop), Body(body)
{
}

csaw::VarStmt::VarStmt(size_t line, const std::string &name, const TypePtr type, ExprPtr value)
        : Stmt(line), Name(name), Type(type), Value(value)
{
}

csaw::WhileStmt::WhileStmt(size_t line, ExprPtr condition, StmtPtr body)
        : Stmt(line), Condition(condition), Body(body)
{
}

csaw::IfStmt::IfStmt(size_t line, ExprPtr condition, StmtPtr _true, StmtPtr _false)
        : Stmt(line), Condition(condition), True(_true), False(_false)
{
}

csaw::ThingStmt::ThingStmt(size_t line,
                           const std::string &name,
                           const std::string &group,
                           const std::map<std::string, TypePtr> &elements)
        : Stmt(line), Name(name), Group(group), Elements(elements)
{
}

csaw::AliasStmt::AliasStmt(size_t line, const std::string &name, const TypePtr origin)
        : Stmt(line), Name(name), Origin(origin)
{
}
