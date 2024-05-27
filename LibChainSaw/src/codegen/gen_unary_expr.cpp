#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::Builder::Gen(const UnaryExpression& expression)
{
    const auto value = Gen(expression.Value);
    if (!value)
        return nullptr;

    const auto op = expression.Operator;

    const auto lvalue = std::dynamic_pointer_cast<LValue>(value);
    if (value->IsLValue())
    {
        if (expression.OpRight) // get and op (@(++):vec3(unused: int1): vec3 ...)
        {
            if (const auto [function, signature] = FindFunction(op, value->GetType(), {Type::GetInt1()}); function)
            {
                const auto result = GetBuilder().CreateCall(function->getFunctionType(), function, {lvalue->GetPointer(), GetBuilder().getInt1(true)});
                return RValue::Create(signature.Result, result);
            }
        }
        else // op and get (@(++):vec3:vec3 ...)
        {
            if (const auto [function, signature] = FindFunction(op, value->GetType(), {}); function)
            {
                const auto result = GetBuilder().CreateCall(function->getFunctionType(), function, {lvalue->GetPointer()});
                return RValue::Create(signature.Result, result);
            }
        }
    }

    const auto rvalue = value->GetRValue();
    bool assign = false;

    RValuePtr result;
    if (op == "-") result = GenNeg(rvalue);
    else if (op == "!") result = GenNot(rvalue);
    else if (op == "~") result = GenInv(rvalue);
    else if (op == "++")
    {
        result = GenInc(rvalue);
        assign = true;
    }
    else if (op == "--")
    {
        result = GenDec(rvalue);
        assign = true;
    }

    if (!result)
    {
        const auto ty = value->GetType()->Name;
        const std::string left = expression.OpRight ? ty : op;
        const std::string right = expression.OpRight ? op : ty;
        ThrowErrorStmt(expression, false, "Unary operation '%s%s' is not implemented", left.c_str(), right.c_str());
        return nullptr;
    }

    if (assign)
    {
        if (!value->IsLValue())
        {
            ThrowErrorStmt(expression, false, "Cannot assign to rvalue");
            return nullptr;
        }

        const auto cast = Cast(result, lvalue->GetType());
        if (!cast)
        {
            ThrowErrorStmt(expression, false, "Failed to cast: %s", cast.Msg().c_str());
            return nullptr;
        }

        lvalue->StoreValue(cast.Get()->GetValue());
        if (expression.OpRight) result = rvalue;
    }

    return result;
}
