#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::ValuePtr csaw::Builder::Gen(const MemberExpression& expression, const TypePtr& expected)
{
    const auto object = Gen(expression.Object, nullptr);
    if (!object)
        return nullptr;

    LValuePtr lobject;
    if (expression.ShouldDeref)
    {
        const auto deref = object->Dereference();
        if (AssertStmt(deref, expression, false, "Failed to dereference: %s", deref.Msg().c_str()))
            return nullptr;

        lobject = deref.Get();
    }
    else if (object->IsLValue())
    {
        lobject = std::dynamic_pointer_cast<LValue>(object);
    }
    else
    {
        const auto alloc = LValue::AllocateAndStore(this, object->GetType(), object);
        if (AssertStmt(alloc, expression, false, "Failed to allocate: %s", alloc.Msg().c_str()))
            return nullptr;

        lobject = alloc.Get();
    }

    if (AssertStmt(lobject->GetType()->IsStruct(), expression, false, "Object must have struct type"))
        return nullptr;

    const auto [eindex, etype] = lobject->GetType()->AsStruct()->GetElement(expression.Member);

    if (eindex < 0 || !etype)
    {
        if (expected && expected->IsFunctionPointer())
        {
            const auto fnty = expected->AsPointer()->Base->AsFunction();

            Signature sig;
            sig.Name = expression.Member;
            sig.Parent = lobject->GetType();
            sig.Args = fnty->Args;
            sig.IsVarargs = fnty->IsVararg;
            sig.Result = fnty->Result;

            const auto [function, signature] = FindBestFunctionBySignature(sig);
            if (AssertStmt(function, expression, false, "Failed to find function by signature %s", sig.Mangle(false).c_str()))
                return nullptr;

            return RValue::Create(this, PointerType::Get(signature.GetFunctionType()), function);
        }

        ThrowErrorStmt(expression, false, "Struct type %s does not have member '%s'", lobject->GetType()->Name.c_str(), expression.Member.c_str());
        return nullptr;
    }

    const auto type = Gen(lobject->GetType());
    if (AssertStmt(type, expression, false, "Failed to generate type %s: %s", lobject->GetType()->Name.c_str(), type.Msg().c_str()))
        return nullptr;

    const auto pointer = GetBuilder().CreateStructGEP(type.Get(), lobject->GetPointer(), eindex);
    return LValue::Direct(this, etype, pointer);
}
