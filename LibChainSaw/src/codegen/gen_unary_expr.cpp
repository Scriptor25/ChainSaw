#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::Builder::Gen(const UnaryExpression& expression)
{
    const auto value = Gen(expression.Value, nullptr);
    if (!value)
        return nullptr;

    const auto op = expression.Operator;

    const auto lvalue = std::dynamic_pointer_cast<LValue>(value);
    if (value->IsLValue())
    {
        if (expression.OpRight) // get and op (@(++):vec3(unused: int1): vec3 ...)
        {
            if (const auto result = FindBestAndCall(op, lvalue, {RValue::Create(this, Type::GetInt1(), GetBuilder().getInt1(true))}))
                return result.Get();
        }
        else // op and get (@(++):vec3:vec3 ...)
        {
            if (const auto result = FindBestAndCall(op, lvalue, {}))
                return result.Get();
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
        if (AssertStmt(value->IsLValue(), expression, false, "Cannot assign to rvalue"))
            return nullptr;

        if (const auto store = lvalue->StoreValue(result);
            AssertStmt(store, expression, false, "Failed to store: %s", store.Msg().c_str()))
            return nullptr;

        if (expression.OpRight) result = rvalue;
    }

    return result;
}
