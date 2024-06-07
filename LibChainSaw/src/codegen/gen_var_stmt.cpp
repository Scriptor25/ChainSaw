#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Stmt.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

void csaw::Builder::Gen(const VariableStatement& statement)
{
    const auto internal = std::ranges::find(statement.Mods, "internal") != statement.Mods.end();

    const auto type = Gen(statement.Type);
    if (AssertStmt(type, statement, false, "Failed to generate type %s: %s", statement.Type->Name.c_str(), type.Msg().c_str()))
        return;

    if (internal && AssertStmt(statement.Value, statement, false, "Failed to create global constant because no initializer is present"))
        return;

    ValuePtr value;
    if (statement.Value)
    {
        value = Gen(statement.Value, statement.Type);
        if (!value)
            return;

        if (const auto result = FindBestCtorAndCall(statement.Type->Name, {value}))
            value = result.Get();
        else if (AssertStmt(result.Msg().empty(), statement, false, "Failed to call constructor: %s", result.Msg().c_str()))
            return;
    }
    else
    {
        if (const auto result = FindBestCtorAndCall(statement.Type->Name, {}))
            value = result.Get();
        else if (AssertStmt(result.Msg().empty(), statement, false, "Failed to call constructor: %s", result.Msg().c_str()))
            return;
    }

    if (value)
    {
        const auto cast = Cast(value, statement.Type);
        if (AssertStmt(cast, statement, false, "Failed to cast: %s", cast.Msg().c_str()))
            return;

        value = cast.Get();
    }

    if (m_ScopeStack.empty())
    {
        auto constant = value && !value->IsLValue() ? llvm::dyn_cast<llvm::Constant>(value->GetValue()) : nullptr;
        const bool no_constant = !constant;

        if (no_constant && value)
            constant = llvm::Constant::getNullValue(type.Get());

        auto linkage = llvm::GlobalValue::ExternalLinkage;
        if (internal)
            linkage = llvm::GlobalValue::InternalLinkage;

        const auto pointer = new llvm::GlobalVariable(GetModule(), type.Get(), internal, linkage, constant, statement.Name);
        const auto ref = LValue::Direct(this, statement.Type, pointer);

        if (no_constant && value)
            if (const auto store = ref->StoreValue(value);
                AssertStmt(store, statement, false, "Failed to store: %s", store.Msg().c_str()))
                return;

        auto& dst = m_Values[statement.Name];
        if (dst)
            TellStmt(statement, "Warning: variable definition shadows previous variable '%s'. This may lead to linking issues.", statement.Name.c_str());
        dst = ref;
        return;
    }

    const auto alloc = LValue::AllocateAndStore(this, statement.Type, value);
    if (AssertStmt(alloc, statement, false, "Failed to allocate and store: %s", alloc.Msg().c_str()))
        return;

    m_Values[statement.Name] = alloc.Get();
}
