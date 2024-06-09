#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::ValuePtr csaw::Builder::Gen(const BinaryExpression& expression)
{
    const auto left = Gen(expression.Left, nullptr);
    if (!left)
        return nullptr;

    const auto right = Gen(expression.Right, nullptr);
    if (!right)
        return nullptr;

    auto op = expression.Operator;

    const auto lleft = std::dynamic_pointer_cast<LValue>(left);
    if (left->IsLValue())
    {
        if (const auto result = FindBestAndCall(op, lleft, {right}))
            return result.Get();
        else if (AssertStmt(result.Msg().empty(), expression, false, "Failed to call: %s", result.Msg().c_str()))
            return nullptr;
    }

    if (const auto result = FindBestAndCall(op, nullptr, {left, right}))
        return result.Get();
    else if (AssertStmt(result.Msg().empty(), expression, false, "Failed to call: %s", result.Msg().c_str()))
        return nullptr;

    if (op == "=")
    {
        if (AssertStmt(left->IsLValue(), expression, false, "Cannot assign to rvalue"))
            return nullptr;

        if (const auto store = lleft->StoreValue(right);
            AssertStmt(store, expression, false, "Failed to store: %s", store.Msg().c_str()))
            return nullptr;

        return lleft->GetRValue();
    }

    const auto best_cast = CastToBestOf(left->GetRValue(), right->GetRValue());
    if (AssertStmt(best_cast, expression, false, "Failed to cast: %s", best_cast.Msg().c_str()))
        return nullptr;

    const auto& [lhs, rhs] = best_cast.Get();

    RValuePtr value;
    if (op == "==") value = GenCmpEQ(lhs, rhs);
    else if (op == "!=") value = GenCmpNE(lhs, rhs);
    else if (op == "<=") value = GenCmpLE(lhs, rhs);
    else if (op == ">=") value = GenCmpGE(lhs, rhs);
    else
    {
        const auto equ_idx = op.find('=');
        const auto assign = equ_idx != std::string::npos;
        if (assign) op = op.substr(0, equ_idx);

        if (op == "&") value = GenAnd(lhs, rhs);
        else if (op == "&&") value = GenLogicalAnd(lhs, rhs);
        else if (op == "|") value = GenOr(lhs, rhs);
        else if (op == "||") value = GenLogicalOr(lhs, rhs);
        else if (op == "^") value = GenXor(lhs, rhs);
        else if (op == "<") value = GenCmpLT(lhs, rhs);
        else if (op == ">") value = GenCmpGT(lhs, rhs);
        else if (op == "<<") value = GenShl(lhs, rhs);
        else if (op == ">>") value = GenAShr(lhs, rhs);
        else if (op == ">>>") value = GenLShr(lhs, rhs);
        else if (op == "+") value = GenAdd(lhs, rhs);
        else if (op == "-") value = GenSub(lhs, rhs);
        else if (op == "*") value = GenMul(lhs, rhs);
        else if (op == "/") value = GenDiv(lhs, rhs);
        else if (op == "%") value = GenRem(lhs, rhs);

        if (value && assign)
        {
            if (AssertStmt(left->IsLValue(), expression, false, "Cannot assign to rvalue"))
                return nullptr;

            if (const auto store = lleft->StoreValue(value);
                AssertStmt(store, expression, false, "Failed to store: %s", store.Msg().c_str()))
                return nullptr;

            return lleft->GetRValue();
        }
    }

    if (AssertStmt(value, expression, false, "Binary operation '%s %s %s' is not implemented", left->GetType()->Name.c_str(), expression.Operator.c_str(), right->GetType()->Name.c_str()))
        return nullptr;

    return value;
}
