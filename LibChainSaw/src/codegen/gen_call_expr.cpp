#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::ValuePtr csaw::Builder::Gen(const CallExpression& expression, const TypePtr& expected)
{
    LValuePtr lparent;
    if (const auto mem_expr = std::dynamic_pointer_cast<MemberExpression>(expression.Callee))
    {
        const auto parent = Gen(mem_expr->Object, nullptr);
        if (!parent)
            return nullptr;

        if (mem_expr->ShouldDeref)
        {
            const auto deref = parent->Dereference();
            if (AssertStmt(deref, *mem_expr, false, "Failed to dereference: %s", deref.Msg().c_str()))
                return nullptr;

            lparent = deref.Get();
        }
        else if (parent->IsLValue())
        {
            lparent = std::dynamic_pointer_cast<LValue>(parent);
        }
        else
        {
            const auto alloc = LValue::AllocateAndStore(this, parent->GetType(), parent);
            if (AssertStmt(alloc, *mem_expr, false, "Failed to allocate and store: %s", alloc.Msg().c_str()))
                return nullptr;

            lparent = alloc.Get();
        }
    }

    std::vector<TypePtr> arg_types;
    auto symbols = GetSymbols();
    for (const auto& arg : expression.Args)
        arg_types.push_back(arg->GetType(symbols, false, {}, {}));

    const auto callee_type = PointerType::Get(FunctionType::Get(arg_types, false, lparent ? lparent->GetType() : nullptr, expected ? expected : Type::GetVoid()));
    const auto callee = Gen(expression.Callee, callee_type);
    if (!callee)
        return nullptr;

    if (AssertStmt(callee->GetType()->IsFunctionPointer(), expression, false, "Callee is not a function pointer"))
        return nullptr;

    Signature signature;
    if (!callee->IsLValue())
        signature = m_Signatures[llvm::cast<llvm::Function>(callee->GetValue())];
    else
    {
        const auto fnty = callee->GetType()->AsPointer()->Base->AsFunction();
        signature.Args = fnty->Args;
        signature.IsVarargs = fnty->IsVararg;
        signature.Result = fnty->Result;
    }

    std::vector<ValuePtr> args;
    for (const auto& arg : expression.Args)
    {
        const auto value = Gen(arg, nullptr);
        if (!value) return nullptr;
        args.push_back(value);
    }

    if (signature.IsConstructor())
    {
        const auto call = CreateCtorCall(callee, Type::Get(signature.Name), args);
        if (AssertStmt(call, expression, false, "Failed to call constructor: %s", call.Msg().c_str()))
            return nullptr;

        return call.Get();
    }

    const auto call = CreateCall(callee, lparent, args);
    if (AssertStmt(call, expression, false, "Failed to call function: %s", call.Msg().c_str()))
        return nullptr;

    return call.Get();
}
