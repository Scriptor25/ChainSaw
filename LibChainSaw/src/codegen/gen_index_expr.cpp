#include <csaw/Error.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Signature.hpp>
#include <csaw/codegen/Value.hpp>
#include <csaw/lang/Expr.hpp>

csaw::LValuePtr csaw::Builder::Gen(const IndexExpression& expression)
{
    const auto array = Gen(expression.Array);
    if (!array)
        return nullptr;

    const auto index = Gen(expression.Index);
    if (!index)
        return nullptr;

    if (const auto [signature, function] = FindFunction("[]", array->GetType(), {index->GetType()}); function)
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

        const auto value = m_Builder->CreateCall(function->getFunctionType(), function, {larray->GetPointer(), cast_index.Get()->GetValue()});
        return LValue::Direct(this, signature.Result->AsPointer()->Base, value);
    }

    if (!array->GetType()->IsPointer())
    {
        ThrowErrorStmt(expression, false, "Cannot index into value with non-pointer type %s", array->GetType()->Name.c_str());
        return nullptr;
    }

    const auto type = array->GetType()->AsPointer()->Base;
    const auto vty = Gen(type);
    if (!vty)
    {
        ThrowErrorStmt(expression, false, "Failed to generate type %s: %s", type->Name.c_str(), vty.Msg().c_str());
        return nullptr;
    }

    const auto pointer = m_Builder->CreateGEP(vty.Get(), array->GetValue(), {index->GetValue()});
    return LValue::Direct(this, type, pointer);
}
