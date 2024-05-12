#include <ostream>
#include <string>
#include <csaw/CSaw.hpp>
#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Stmt.hpp>

static size_t depth = 0;
static bool end = true;

static std::string spaces()
{
    std::string spaces;
    for (size_t i = 0; i < depth; i++)
        spaces += "    ";
    return spaces;
}

std::ostream& csaw::operator<<(std::ostream& out, const StatementPtr& ptr)
{
    if (const auto stmt = std::dynamic_pointer_cast<ScopeStatement>(ptr))
        return out << *stmt;
    if (const auto stmt = std::dynamic_pointer_cast<ForStatement>(ptr))
        return out << *stmt;
    if (const auto stmt = std::dynamic_pointer_cast<FunctionStatement>(ptr))
        return out << *stmt;
    if (const auto stmt = std::dynamic_pointer_cast<IfStatement>(ptr))
        return out << *stmt;
    if (const auto stmt = std::dynamic_pointer_cast<RetStatement>(ptr))
        return out << *stmt;
    if (const auto stmt = std::dynamic_pointer_cast<DefStatement>(ptr))
        return out << *stmt;
    if (const auto stmt = std::dynamic_pointer_cast<VariableStatement>(ptr))
        return out << *stmt;
    if (const auto stmt = std::dynamic_pointer_cast<WhileStatement>(ptr))
        return out << *stmt;

    if (const auto expr = std::dynamic_pointer_cast<Expression>(ptr))
    {
        out << expr;
        if (end) return out << ';';
        return out;
    }

    CSAW_MESSAGE_STMT(true, *ptr, "printing is not implemented for this statement");
}

std::ostream& csaw::operator<<(std::ostream& out, const ScopeStatement& statement)
{
    depth++;
    const auto sp = spaces();
    out << '{' << '\n';
    for (auto& s : statement.Body)
        out << sp << s << '\n';
    depth--;
    return out << spaces() << '}';
}

std::ostream& csaw::operator<<(std::ostream& out, const ForStatement& statement)
{
    out << "for (";
    if (statement.Pre) out << statement.Pre;
    else out << ';';
    out << ' ';
    if (statement.Condition) out << statement.Condition;
    out << "; ";
    end = false;
    if (statement.Loop) out << statement.Loop;
    end = true;
    return out << ") " << statement.Body;
}

std::ostream& csaw::operator<<(std::ostream& out, const FunctionStatement& statement)
{
    if (statement.IsConstructor) out << '$';
    else out << '@';

    bool op = false;
    for (const auto c : statement.Name)
        if (!isalnum(c) && c != '_')
        {
            op = true;
            break;
        }
    if (op) out << '(';
    out << statement.Name;
    if (op) out << ')';

    if (statement.Callee)
        out << ':' << statement.Callee;

    if (!statement.Args.empty() || statement.IsVarArg)
    {
        out << '(';
        for (size_t i = 0; i < statement.Args.size(); i++)
        {
            if (i > 0)
                out << ", ";
            out << statement.Args[i].first << ": " << statement.Args[i].second;
        }
        if (statement.IsVarArg)
            out << ", ?";
        out << ')';
    }

    if (!statement.IsConstructor && statement.Result)
        out << (statement.Args.empty() && !statement.IsVarArg && !statement.Callee ? "::" : ": ") << statement.Result;

    if (!statement.Body)
        return out << ';';

    if (!std::dynamic_pointer_cast<ScopeStatement>(statement.Body))
        out << " =";

    return out << ' ' << statement.Body;
}

std::ostream& csaw::operator<<(std::ostream& out, const IfStatement& statement)
{
    out << "if (" << statement.Condition << ") " << statement.True;
    if (statement.False) out << " else " << statement.False;
    return out;
}

std::ostream& csaw::operator<<(std::ostream& out, const RetStatement& statement)
{
    out << "ret";
    if (statement.Value)
        out << ' ' << statement.Value;

    if (end)
        return out << ';';
    return out;
}

std::ostream& csaw::operator<<(std::ostream& out, const DefStatement& statement)
{
    out << "def " << statement.Name;

    if (statement.Origin)
        return out << " = " << statement.Origin << ';';

    if (statement.Elements.empty())
        return out << ';';

    depth++;
    const auto sp = spaces();
    out << " {" << '\n';
    size_t i = 0;
    for (const auto& [name, type] : statement.Elements)
    {
        if (i++ > 0) out << ",\n";
        out << sp << name << ": " << type;
    }
    depth--;
    return out << spaces() << "\n}";
}

std::ostream& csaw::operator<<(std::ostream& out, const VariableStatement& statement)
{
    out << statement.Name << ": " << statement.Type;
    if (!statement.Value)
    {
        if (end) out << ';';
        return out;
    }
    out << " = " << statement.Value;
    if (end) out << ';';
    return out;
}

std::ostream& csaw::operator<<(std::ostream& out, const WhileStatement& statement)
{
    return out << "while (" << statement.Condition << ") " << statement.Body;
}
