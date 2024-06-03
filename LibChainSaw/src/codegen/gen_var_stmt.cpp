#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Stmt.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

void csaw::Builder::Gen(const VariableStatement& statement)
{
    const auto is_const = std::ranges::find(statement.Mods, "const") != statement.Mods.end();

    const auto type = Gen(statement.Type);
    if (AssertStmt(type, statement, false, "Failed to generate type %s: %s", statement.Type->Name.c_str(), type.Msg().c_str()))
        return;

    if (is_const && AssertStmt(statement.Value, statement, false, "Failed to create global constant because no initializer is present"))
        return;

    ValuePtr initializer;
    if (statement.Value)
    {
        initializer = Gen(statement.Value, statement.Type);
        if (!initializer)
            return;

        if (const auto result = FindBestCtorAndCall(statement.Type->Name, {initializer}))
            initializer = result.Get();
        else if (!result.Msg().empty())
            return ThrowErrorStmt(statement, false, "Failed to call constructor: %s", result.Msg().c_str());
    }
    else
    {
        if (const auto result = FindBestCtorAndCall(statement.Type->Name, {}))
            initializer = result.Get();
        else if (!result.Msg().empty())
            return ThrowErrorStmt(statement, false, "Failed to call constructor: %s", result.Msg().c_str());
    }

    if (initializer)
    {
        const auto cast = Cast(initializer, statement.Type);
        if (AssertStmt(cast, statement, false, "Failed to cast: %s", cast.Msg().c_str()))
            return;

        initializer = cast.Get();
    }

    if (m_ScopeStack.empty())
    {
        auto global_initializer = initializer && !initializer->IsLValue() ? llvm::dyn_cast<llvm::Constant>(initializer->GetValue()) : nullptr;
        const bool no_global_initializer = !global_initializer;

        if (no_global_initializer && initializer)
            global_initializer = llvm::Constant::getNullValue(type.Get());

        const auto pointer = new llvm::GlobalVariable(GetModule(), type.Get(), is_const, is_const ? llvm::GlobalValue::InternalLinkage : llvm::GlobalValue::ExternalLinkage, global_initializer, statement.Name);
        const auto ref = LValue::Direct(this, statement.Type, pointer);

        if (no_global_initializer && initializer)
            if (const auto store = ref->StoreValue(initializer);
                AssertStmt(store, statement, false, "Failed to store: %s", store.Msg().c_str()))
                return;

        m_Values[statement.Name] = ref;
        return;
    }

    const auto alloc = LValue::AllocateAndStore(this, statement.Type, initializer);
    if (!alloc)
        return ThrowErrorStmt(statement, false, "Failed to allocate and store: %s", alloc.Msg().c_str());

    m_Values[statement.Name] = alloc.Get();
}
