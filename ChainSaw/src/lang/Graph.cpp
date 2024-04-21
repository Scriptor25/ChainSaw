#include <lang/Def.hpp>
#include <lang/Expr.hpp>
#include <lang/Stmt.hpp>
#include <lang/Type.hpp>

Agnode_t *csaw::lang::Graph(Agraph_t *g, const StmtPtr ptr)
{
    if (!ptr)
        return nullptr;

    if (auto stmt = std::dynamic_pointer_cast<FunStmt>(ptr))
        return Graph(g, *stmt);
    if (auto stmt = std::dynamic_pointer_cast<RetStmt>(ptr))
        return Graph(g, *stmt);
    if (auto stmt = std::dynamic_pointer_cast<EnclosedStmt>(ptr))
        return Graph(g, *stmt);
    if (auto stmt = std::dynamic_pointer_cast<ForStmt>(ptr))
        return Graph(g, *stmt);
    if (auto stmt = std::dynamic_pointer_cast<VarStmt>(ptr))
        return Graph(g, *stmt);
    if (auto stmt = std::dynamic_pointer_cast<WhileStmt>(ptr))
        return Graph(g, *stmt);
    if (auto stmt = std::dynamic_pointer_cast<IfStmt>(ptr))
        return Graph(g, *stmt);
    if (auto stmt = std::dynamic_pointer_cast<ThingStmt>(ptr))
        return Graph(g, *stmt);
    if (auto stmt = std::dynamic_pointer_cast<AliasStmt>(ptr))
        return Graph(g, *stmt);

    if (auto expr = std::dynamic_pointer_cast<Expr>(ptr))
        return Graph(g, expr);

    throw std::runtime_error("Not yet implemented");
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::FunStmt &stmt)
{
    if (stmt.Callee)
    {
        auto clusterName = "cluster_" + stmt.Callee->Name;
        g = agsubg(g, (char *) clusterName.c_str(), TRUE);
        {
            agsafeset(g, (char *) "label", stmt.Callee->Name.c_str(), "");
        }
    }

    auto name = stmt.Name;
    name += " (";
    for (size_t i = 0; i < stmt.Args.size(); ++i)
    {
        if (i > 0) name += ", ";
        name += stmt.Args[i].first + ": " + stmt.Args[i].second->Name;
    }
    if (!stmt.VarArg.empty())
        name += ", " + stmt.VarArg + '?';
    name += ')';

    auto clusterName = "cluster_" + name;
    g = agsubg(g, (char *) clusterName.c_str(), TRUE);
    {
        agsafeset(g, (char *) "label", name.c_str(), "");
    }

    if (!stmt.Body)
    {
        auto n = agnode(g, nullptr, TRUE);
        agsafeset(n, (char *) "style", "invis", "");
        return n;
    }

    return Graph(g, *stmt.Body);
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::RetStmt &stmt)
{
    auto n = agnode(g, nullptr, TRUE);
    agsafeset(n, (char *) "label", "ret", "\\N");
    if (stmt.Value)
    {
        auto h = Graph(g, stmt.Value);
        agedge(g, n, h, nullptr, TRUE);
    }
    return n;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::EnclosedStmt &stmt)
{
    static size_t idx = 0;

    auto name = "cluster_enclosed" + std::to_string(idx++);
    g = agsubg(g, (char *) name.c_str(), TRUE);
    {
        agsafeset(g, (char *) "label", "", "");
    }

    Agnode_t *first = nullptr;
    Agnode_t *p = nullptr;
    for (auto ptr: stmt.Content)
    {
        auto n = Graph(g, ptr);
        if (!first) first = n;
        if (p)
        {
            auto e = agedge(g, p, n, nullptr, TRUE);
            agsafeset(e, (char *) "color", "#f00", "black");
        }
        p = n;
    }

    return first;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::ForStmt &stmt)
{
    return agnode(g, nullptr, TRUE);
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::VarStmt &stmt)
{
    return agnode(g, nullptr, TRUE);
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::WhileStmt &stmt)
{
    return agnode(g, nullptr, TRUE);
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::IfStmt &stmt)
{
    auto n = agnode(g, nullptr, TRUE);
    agsafeset(n, (char *) "label", "if", "\\N");

    auto c = Graph(g, stmt.Condition);
    auto t = Graph(g, stmt.True);
    auto f = Graph(g, stmt.False);

    agedge(g, n, c, nullptr, TRUE);
    agedge(g, n, t, nullptr, TRUE);
    if (f) agedge(g, n, f, nullptr, TRUE);

    return n;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::ThingStmt &stmt)
{
    return agnode(g, nullptr, TRUE);
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::AliasStmt &stmt)
{
    std::string name = "alias " + stmt.Name + ": " + stmt.Origin->Name;
    return agnode(g, (char *) name.c_str(), TRUE);
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::ExprPtr ptr)
{
    if (!ptr)
        return nullptr;

    if (auto expr = std::dynamic_pointer_cast<CallExpr>(ptr))
        return Graph(g, *expr);
    if (auto expr = std::dynamic_pointer_cast<NumExpr>(ptr))
        return Graph(g, *expr);
    if (auto expr = std::dynamic_pointer_cast<ChrExpr>(ptr))
        return Graph(g, *expr);
    if (auto expr = std::dynamic_pointer_cast<StrExpr>(ptr))
        return Graph(g, *expr);
    if (auto expr = std::dynamic_pointer_cast<IdentExpr>(ptr))
        return Graph(g, *expr);
    if (auto expr = std::dynamic_pointer_cast<BinExpr>(ptr))
        return Graph(g, *expr);
    if (auto expr = std::dynamic_pointer_cast<UnExpr>(ptr))
        return Graph(g, *expr);
    if (auto expr = std::dynamic_pointer_cast<IndexExpr>(ptr))
        return Graph(g, *expr);
    if (auto expr = std::dynamic_pointer_cast<MemberExpr>(ptr))
        return Graph(g, *expr);
    if (auto expr = std::dynamic_pointer_cast<VarArgExpr>(ptr))
        return Graph(g, *expr);
    if (auto expr = std::dynamic_pointer_cast<SelExpr>(ptr))
        return Graph(g, *expr);

    throw;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::CallExpr &expr)
{
    expr.Callee;

    std::string name;
    auto callee = expr.Callee;
    while (true)
    {
        if (auto iPtr = std::dynamic_pointer_cast<IdentExpr>(callee))
        {
            name = iPtr->Id + name;
            break;
        }
        else if (auto mPtr = std::dynamic_pointer_cast<MemberExpr>(callee))
        {
            name = "." + mPtr->Member + name;
            callee = mPtr->Object;
        }
    }

    auto n = agnode(g, nullptr, TRUE);
    agsafeset(n, (char *) "label", name.c_str(), "\\N");

    for (auto arg: expr.Args)
    {
        auto an = Graph(g, arg);
        agedge(g, n, an, nullptr, TRUE);
    }

    return n;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::NumExpr &expr)
{
    auto name = std::to_string(expr.Value);
    auto n = agnode(g, nullptr, TRUE);
    agsafeset(n, (char *) "label", name.c_str(), "\\N");
    return n;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::ChrExpr &expr)
{
    auto name = std::string(1, expr.Value);
    auto n = agnode(g, nullptr, TRUE);
    agsafeset(n, (char *) "label", name.c_str(), "\\N");
    return n;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::StrExpr &expr)
{
    auto name = '"' + expr.Value + '"';
    auto n = agnode(g, nullptr, TRUE);
    agsafeset(n, (char *) "label", name.c_str(), "\\N");
    return n;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::IdentExpr &expr)
{
    auto n = agnode(g, nullptr, TRUE);
    agsafeset(n, (char *) "label", expr.Id.c_str(), "\\N");
    return n;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::BinExpr &expr)
{
    auto n = agnode(g, nullptr, TRUE);
    agsafeset(n, (char *) "label", expr.Operator.c_str(), "\\N");

    auto l = Graph(g, expr.Left);
    auto r = Graph(g, expr.Right);

    agedge(g, n, l, nullptr, TRUE);
    agedge(g, n, r, nullptr, TRUE);

    return n;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::UnExpr &expr)
{
    auto n = agnode(g, nullptr, TRUE);
    agsafeset(n, (char *) "label", expr.Operator.c_str(), "\\N");

    auto v = Graph(g, expr.Value);
    agedge(g, n, v, nullptr, TRUE);

    return n;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::IndexExpr &expr)
{
    auto n = agnode(g, nullptr, TRUE);
    agsafeset(n, (char *) "label", "[]", "\\N");

    auto a = Graph(g, expr.Array);
    auto i = Graph(g, expr.Index);

    agedge(g, n, a, nullptr, TRUE);
    agedge(g, n, i, nullptr, TRUE);

    return n;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::MemberExpr &expr)
{
    auto name = '.' + expr.Member;

    auto n = agnode(g, nullptr, TRUE);
    agsafeset(n, (char *) "label", name.c_str(), "\\N");

    auto o = Graph(g, expr.Object);
    agedge(g, o, n, nullptr, TRUE);

    return o;
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::VarArgExpr &expr)
{
    return agnode(g, nullptr, TRUE);
}

Agnode_t *csaw::lang::Graph(Agraph_t *g, const csaw::lang::SelExpr &expr)
{
    return agnode(g, nullptr, TRUE);
}
