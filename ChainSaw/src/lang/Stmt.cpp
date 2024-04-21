#include <lang/Assert.hpp>
#include <lang/Parser.hpp>
#include <lang/Stmt.hpp>
#include <lang/Expr.hpp>

csaw::lang::StmtPtr csaw::lang::Parser::ParseStmt(bool end)
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
    if (auto vstmt = ParseVarStmt(expr, end))
        return vstmt;

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
csaw::lang::FunStmtPtr csaw::lang::Parser::ParseFunStmt(bool end)
{
    auto line = m_Line;

    bool constructor;
    std::string name;
    std::string callee;
    std::vector<std::pair<std::string, std::string>> args;
    std::string vararg;
    std::string result;
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
            callee = Expect(TK_IDENTIFIER).Value;

    if (NextIfAt("("))
    {
        while (!At(")") && !AtEOF())
        {
            std::string aName = Expect(TK_IDENTIFIER).Value;
            if (NextIfAt("?"))
            {
                vararg = aName;
                break;
            }
            Expect(":");
            std::string aType = Expect(TK_IDENTIFIER).Value;
            args.push_back({aName, aType});
            if (!At(")"))
                Expect(",");
        }
        Expect(")");
    }

    if (NextIfAt(":"))
        result = Expect(TK_IDENTIFIER).Value;

    if (!At("{"))
    {
        if (end) Expect(";");
        return std::make_shared<FunStmt>(line, constructor, name, callee, args, vararg, result, body);
    }

    body = ParseEnclosedStmt();
    return std::make_shared<FunStmt>(line, constructor, name, callee, args, vararg, result, body);
}

csaw::lang::RetStmtPtr csaw::lang::Parser::ParseRetStmt(bool end)
{
    auto line = m_Line;

    Expect("ret");

    if (NextIfAt(";"))
        return std::make_shared<RetStmt>(line, ExprPtr());

    auto value = ParseExpr();
    if (end) Expect(";");

    return std::make_shared<RetStmt>(line, value);
}

csaw::lang::EnclosedStmtPtr csaw::lang::Parser::ParseEnclosedStmt()
{
    auto line = m_Line;

    Expect("{");
    std::vector<StmtPtr> content;
    while (!AtEOF() && !At("}"))
        content.push_back(ParseStmt());
    Expect("}");

    return std::make_shared<EnclosedStmt>(line, content);
}

csaw::lang::ForStmtPtr csaw::lang::Parser::ParseForStmt()
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

csaw::lang::VarStmtPtr csaw::lang::Parser::ParseVarStmt(ExprPtr expr, bool end)
{
    auto line = m_Line;

    if (auto type = std::dynamic_pointer_cast<IdentExpr>(expr))
    {
        std::string name = Expect(TK_IDENTIFIER).Value;

        if (!At("="))
        {
            if (end) Expect(";");
            return std::make_shared<VarStmt>(line, type->Id, name, ExprPtr());
        }

        Expect("=");
        auto value = ParseExpr();

        if (end) Expect(";");
        return std::make_shared<VarStmt>(line, type->Id, name, value);
    }

    return VarStmtPtr();
}

csaw::lang::WhileStmtPtr csaw::lang::Parser::ParseWhileStmt()
{
    auto line = m_Line;

    Expect("while");
    Expect("(");

    auto condition = ParseExpr();

    Expect(")");

    auto body = ParseStmt();

    return std::make_shared<WhileStmt>(line, condition, body);
}

csaw::lang::IfStmtPtr csaw::lang::Parser::ParseIfStmt()
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

csaw::lang::ThingStmtPtr csaw::lang::Parser::ParseThingStmt(bool end)
{
    auto line = m_Line;

    Expect("thing");
    Expect(":");
    std::string name = Expect(TK_IDENTIFIER).Value;

    std::string group;
    if (NextIfAt(":"))
        group = Expect(TK_IDENTIFIER).Value;

    std::map<std::string, std::string> elements;
    if (!At("{"))
    {
        if (end) Expect(";");
        return std::make_shared<ThingStmt>(line, name, group, elements);
    }

    Expect("{");
    while (!AtEOF() && !At("}"))
    {
        std::string ename = Expect(TK_IDENTIFIER).Value;
        Expect(":");
        std::string etype = Expect(TK_IDENTIFIER).Value;
        elements[ename] = etype;

        if (!At("}"))
            Expect(",");
    }
    Expect("}");

    return std::make_shared<ThingStmt>(line, name, group, elements);
}

csaw::lang::AliasStmtPtr csaw::lang::Parser::ParseAliasStmt(bool end)
{
    auto line = m_Line;

    Expect("alias");
    std::string name = Expect(TK_IDENTIFIER).Value;
    Expect(":");
    std::string origin = Expect(TK_IDENTIFIER).Value;

    if (end) Expect(";");
    return std::make_shared<AliasStmt>(line, name, origin);
}

csaw::lang::Stmt::Stmt(size_t line)
        : Line(line)
{
}

csaw::lang::EnclosedStmt::EnclosedStmt(size_t line, const std::vector<StmtPtr> &content)
        : Stmt(line), Content(content)
{
}

csaw::lang::FunStmt::FunStmt(size_t line,
                             bool constructor,
                             const std::string &name,
                             const std::string &callee,
                             const std::vector<std::pair<std::string, std::string>> &args,
                             const std::string &vararg,
                             const std::string &result,
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

csaw::lang::RetStmt::RetStmt(size_t line, ExprPtr value)
        : Stmt(line), Value(value)
{
}

csaw::lang::ForStmt::ForStmt(size_t line, StmtPtr pre, ExprPtr condition, StmtPtr loop, StmtPtr body)
        : Stmt(line), Pre(pre), Condition(condition), Loop(loop), Body(body)
{
}

csaw::lang::VarStmt::VarStmt(size_t line, const std::string &type, const std::string &name, ExprPtr value)
        : Stmt(line), Type(type), Name(name), Value(value)
{
}

csaw::lang::WhileStmt::WhileStmt(size_t line, ExprPtr condition, StmtPtr body)
        : Stmt(line), Condition(condition), Body(body)
{
}

csaw::lang::IfStmt::IfStmt(size_t line, ExprPtr condition, StmtPtr _true, StmtPtr _false)
        : Stmt(line), Condition(condition), True(_true), False(_false)
{
}

csaw::lang::ThingStmt::ThingStmt(size_t line, const std::string &name, const std::string &group,
                                 const std::map<std::string, std::string> &elements)
        : Stmt(line), Name(name), Group(group), Elements(elements)
{
}

csaw::lang::AliasStmt::AliasStmt(size_t line, const std::string &name, const std::string &origin)
        : Stmt(line), Name(name), Origin(origin)
{
}
