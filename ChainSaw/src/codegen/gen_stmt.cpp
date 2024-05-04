#include <csaw/codegen/Builder.hpp>
#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Stmt.hpp>

void csaw::Builder::Gen(const StatementPtr& ptr)
{
    if (const auto p = std::dynamic_pointer_cast<ScopeStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<ForStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<FunctionStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<IfStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<RetStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<DefStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<VariableStatement>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<WhileStatement>(ptr))
        return Gen(*p);

    if (const auto p = std::dynamic_pointer_cast<Expression>(ptr))
    {
        Gen(p);
        return;
    }

    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const ScopeStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const ForStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const FunctionStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const IfStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const RetStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const DefStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const VariableStatement& statement)
{
    const auto type = Gen(statement.Type);
    const auto initializer = Gen(statement.Value);

    if (IsGlobal())
    {
        llvm::Constant* global_initializer = nullptr;
        if (initializer)
        {
            global_initializer = llvm::dyn_cast<llvm::Constant>(initializer);
            if (!global_initializer)
                throw std::runtime_error("not yet implemented");
        }

        (void)llvm::GlobalVariable(*m_Module, type, false, llvm::GlobalValue::InternalLinkage, global_initializer, statement.Name);
        return;
    }

    throw std::runtime_error("not yet implemented");
}

void csaw::Builder::Gen(const WhileStatement& statement)
{
    throw std::runtime_error("not yet implemented");
}
