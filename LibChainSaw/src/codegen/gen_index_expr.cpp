#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::LValuePtr csaw::Builder::Gen(const IndexExpression& expression)
{
    const auto array = Gen(expression.Array, nullptr);
    if (!array)
        return nullptr;

    const auto index = Gen(expression.Index, nullptr);
    if (!index)
        return nullptr;

    if (const auto [function, signature] = FindBestFunction("[]", array->GetType(), {index->GetType()}); function)
    {
        if (AssertStmt(signature.Result->IsPointer(), expression, false, "Function overloading the index operator must return a value with pointer type"))
            return nullptr;

        LValuePtr larray;
        if (array->IsLValue())
            larray = std::dynamic_pointer_cast<LValue>(array);
        else
        {
            const auto alloc = LValue::AllocateAndStore(this, array->GetType(), array);
            if (AssertStmt(alloc, expression, false, "Failed to allocate and store: %s", alloc.Msg().c_str()))
                return nullptr;

            larray = alloc.Get();
        }

        const ValuePtr callee = RValue::Create(this, PointerType::Get(signature.GetFunctionType()), function);
        const auto value = CreateCall(callee, larray, {index});
        if (AssertStmt(value, expression, false, "Failed to call function: %s", value.Msg().c_str()))
            return nullptr;

        const auto deref = value.Get()->Dereference();
        if (AssertStmt(deref, expression, false, "Failed to dereference: %s", deref.Msg().c_str()))
            return nullptr;

        return deref.Get();
    }

    if (array->GetType()->IsPointer())
    {
        const auto base = array->GetType()->AsPointer()->Base;
        const auto ty = Gen(base);
        if (AssertStmt(ty, expression, false, "Failed to generate type %s: %s", base->Name.c_str(), ty.Msg().c_str()))
            return nullptr;

        const auto pointer = GetBuilder().CreateGEP(ty.Get(), array->GetValue(), {index->GetValue()});
        return LValue::Direct(this, base, pointer);
    }

    if (array->GetType()->IsArray())
    {
        const auto base = array->GetType()->AsArray()->Base;
        const auto ty = Gen(array->GetType());
        if (AssertStmt(ty, expression, false, "Failed to generate type %s: %s", base->Name.c_str(), ty.Msg().c_str()))
            return nullptr;

        const auto larray = std::dynamic_pointer_cast<LValue>(array);
        const auto pointer = GetBuilder().CreateGEP(ty.Get(), larray->GetPointer(), {GetBuilder().getInt64(0), index->GetValue()});
        return LValue::Direct(this, base, pointer);
    }

    ThrowErrorStmt(expression, false, "Cannot index into value with non-pointer type %s", array->GetType()->Name.c_str());
    return nullptr;
}
