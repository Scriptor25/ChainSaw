#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Stmt.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

void csaw::Builder::Gen(const VariableStatement& statement)
{
    const auto type = Gen(statement.Type);
    if (!type)
        return ThrowErrorStmt(statement, false, "Failed to generate type %s: %s", statement.Type->Name.c_str(), type.Msg().c_str());

    ValuePtr initializer;
    if (statement.Value)
    {
        initializer = Gen(statement.Value);
        if (!initializer)
            return;

        if (const auto [function, signature] = FindFunction(statement.Type->Name, nullptr, {initializer->GetType()}); function && signature.IsConstructor())
        {
            const auto alloc = LValue::Allocate(this, Type::Get(signature.Name));
            if (!alloc)
                return ThrowErrorStmt(statement, false, "Failed to allocate: %s", alloc.Msg().c_str());

            const auto cast = Cast(initializer, signature.Args[0]);
            if (!cast)
                return ThrowErrorStmt(statement, false, "Failed to cast: %s", cast.Msg().c_str());

            initializer = alloc.Get();
            GetBuilder().CreateCall(function->getFunctionType(), function, {alloc.Get()->GetPointer(), cast.Get()->GetValue()});
        }
    }
    else if (const auto [function, signature] = FindFunction(statement.Type->Name, nullptr, {}); function && signature.IsConstructor())
    {
        const auto alloc = LValue::Allocate(this, Type::Get(signature.Name));
        if (!alloc)
            return ThrowErrorStmt(statement, false, "Failed to allocate: %s", alloc.Msg().c_str());

        initializer = alloc.Get();
        GetBuilder().CreateCall(function->getFunctionType(), function, {alloc.Get()->GetPointer()});
    }

    if (initializer)
    {
        const auto cast = Cast(initializer, statement.Type);
        if (!cast)
            return ThrowErrorStmt(statement, false, "Failed to cast: %s", cast.Msg().c_str());

        initializer = cast.Get();
    }

    if (m_ScopeStack.empty())
    {
        llvm::Constant* global_initializer = nullptr;
        if (initializer)
            global_initializer = llvm::dyn_cast<llvm::Constant>(initializer->GetValue());

        const bool is_initialized = global_initializer;
        if (!is_initialized)
            global_initializer = llvm::Constant::getNullValue(type.Get());

        const auto pointer = new llvm::GlobalVariable(GetModule(), type.Get(), false, llvm::GlobalValue::InternalLinkage, global_initializer, statement.Name);
        const auto ref = LValue::Direct(this, statement.Type, pointer);

        if (!is_initialized && initializer)
            ref->StoreValue(initializer->GetValue());

        m_Values[statement.Name] = ref;
        return;
    }

    const auto alloc = LValue::AllocateAndStore(this, statement.Type, initializer ? initializer->GetValue() : llvm::Constant::getNullValue(type.Get()));
    if (!alloc)
        return ThrowErrorStmt(statement, false, "Failed to allocate: %s", alloc.Msg().c_str());

    m_Values[statement.Name] = alloc.Get();
}
