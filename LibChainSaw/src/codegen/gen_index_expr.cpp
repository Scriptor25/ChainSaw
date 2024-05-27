#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::LValuePtr csaw::Builder::Gen(const IndexExpression& expression)
{
    const auto array = Gen(expression.Array);
    if (!array)
        return nullptr;

    const auto index = Gen(expression.Index);
    if (!index)
        return nullptr;

    if (const auto [function, signature] = FindFunction("[]", array->GetType(), {index->GetType()}); function)
    {
        if (!signature.Result->IsPointer())
        {
            ThrowErrorStmt(expression, false, "Function overloading the index operator must return a value with pointer type");
            return nullptr;
        }

        const auto cast_index = Cast(index, signature.Args[0]);
        if (!cast_index)
        {
            ThrowErrorStmt(expression, false, "Failed to cast: %s", cast_index.Msg().c_str());
            return nullptr;
        }

        LValuePtr larray;
        if (array->IsLValue())
            larray = std::dynamic_pointer_cast<LValue>(array);
        else
        {
            const auto alloc = LValue::AllocateAndStore(this, array->GetType(), array->GetValue());
            if (!alloc)
            {
                ThrowErrorStmt(expression, false, "Failed to allocate: %s", alloc.Msg().c_str());
                return nullptr;
            }
            larray = alloc.Get();
        }

        const auto value = GetBuilder().CreateCall(function->getFunctionType(), function, {larray->GetPointer(), cast_index.Get()->GetValue()});
        return LValue::Direct(this, signature.Result->AsPointer()->Base, value);
    }

    if (array->GetType()->IsPointer())
    {
        const auto base = array->GetType()->AsPointer()->Base;
        const auto ty = Gen(base);
        if (!ty)
        {
            ThrowErrorStmt(expression, false, "Failed to generate type %s: %s", base->Name.c_str(), ty.Msg().c_str());
            return nullptr;
        }

        const auto pointer = GetBuilder().CreateGEP(ty.Get(), array->GetValue(), {index->GetValue()});
        return LValue::Direct(this, base, pointer);
    }

    if (array->GetType()->IsArray())
    {
        const auto base = array->GetType()->AsArray()->Base;
        const auto ty = Gen(array->GetType());
        if (!ty)
        {
            ThrowErrorStmt(expression, false, "Failed to generate type %s: %s", base->Name.c_str(), ty.Msg().c_str());
            return nullptr;
        }

        const auto larray = std::dynamic_pointer_cast<LValue>(array);
        const auto pointer = GetBuilder().CreateGEP(ty.Get(), larray->GetPointer(), {GetBuilder().getInt64(0), index->GetValue()});
        return LValue::Direct(this, base, pointer);
    }

    ThrowErrorStmt(expression, false, "Cannot index into value with non-pointer type %s", array->GetType()->Name.c_str());
    return nullptr;
}
