#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::LValuePtr csaw::Builder::Gen(const MemberExpression& expression)
{
    const auto object = Gen(expression.Object);
    if (!object)
        return nullptr;

    LValuePtr lobject;
    if (object->IsLValue())
    {
        lobject = std::dynamic_pointer_cast<LValue>(object);
        if (expression.ShouldDeref)
        {
            const auto deref = lobject->Dereference();
            if (!deref)
            {
                ThrowErrorStmt(expression, false, "Failed to dereference: %s", deref.Msg().c_str());
                return nullptr;
            }
            lobject = deref.Get();
        }
    }
    else
    {
        if (expression.ShouldDeref)
        {
            const auto deref = object->GetRValue()->Dereference(this);
            if (!deref)
            {
                ThrowErrorStmt(expression, false, "Failed to dereference: %s", deref.Msg().c_str());
                return nullptr;
            }
            lobject = deref.Get();
        }
        else
        {
            const auto alloc = LValue::AllocateAndStore(this, object->GetType(), object->GetValue());
            if (!alloc)
            {
                ThrowErrorStmt(expression, false, "Failed to allocate: %s", alloc.Msg().c_str());
                return nullptr;
            }
            lobject = alloc.Get();
        }
    }

    if (!lobject->GetType()->IsStruct())
    {
        ThrowErrorStmt(expression, false, "Object must have struct type");
        return nullptr;
    }

    const auto [eindex, etype] = lobject->GetType()->AsStruct()->GetElement(expression.Member);

    if (eindex < 0 || !etype)
    {
        ThrowErrorStmt(expression, false, "Struct type %s does not have member '%s'", lobject->GetType()->Name.c_str(), expression.Member.c_str());
        return nullptr;
    }

    const auto type = Gen(lobject->GetType());
    if (!type)
    {
        ThrowErrorStmt(expression, false, "Failed to generate type %s: %s", lobject->GetType()->Name.c_str(), type.Msg().c_str());
        return nullptr;
    }

    const auto pointer = GetBuilder().CreateStructGEP(type.Get(), lobject->GetPointer(), eindex);
    return LValue::Direct(this, etype, pointer);
}
