#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::ValuePtr csaw::Builder::Gen(const IdentifierExpression& expression, const TypePtr& expected)
{
    if (expected && expected->IsFunctionPointer())
    {
        const auto fnty = expected->AsPointer()->Base->AsFunction();

        Signature sig;
        sig.Name = expression.Id;
        sig.Args = fnty->Args;
        sig.IsVarargs = fnty->IsVararg;
        sig.Result = fnty->Result;

        const auto [function, signature] = FindBestFunctionBySignature(sig);
        if (AssertStmt(function, expression, false, "Failed to find function by signature %s", sig.Mangle(false).c_str()))
            return nullptr;

        return RValue::Create(this, PointerType::Get(signature.GetFunctionType()), function);
    }

    if (const auto& value = m_Values[expression.Id])
        return value;

    ThrowErrorStmt(expression, false, "Undefined identifier '%s'", expression.Id.c_str());
    return nullptr;
}
