#include <csaw/Error.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Signature.hpp>
#include <csaw/codegen/Value.hpp>
#include <csaw/lang/Expr.hpp>

csaw::RValuePtr csaw::Builder::Gen(const BinaryExpression& expression)
{
    const auto left = Gen(expression.Left);
    if (!left)
        return nullptr;

    const auto right = Gen(expression.Right);
    if (!right)
        return nullptr;

    auto op = expression.Operator;

    if (const auto [signature, function] = FindFunction(op, nullptr, {left->GetType(), right->GetType()}); function)
    {
        const auto cast_left = Cast(left, signature.Args[0]);
        if (!cast_left)
        {
            ThrowErrorStmt(expression, false, "Failed to cast: %s", cast_left.Msg().c_str());
            return nullptr;
        }

        const auto cast_right = Cast(right, signature.Args[1]);
        if (!cast_right)
        {
            ThrowErrorStmt(expression, false, "Failed to cast: %s", cast_right.Msg().c_str());
            return nullptr;
        }

        const auto value = m_Builder->CreateCall(function->getFunctionType(), function, {cast_left.Get()->GetValue(), cast_right.Get()->GetValue()});
        return RValue::Create(signature.Result, value);
    }

    const auto lleft = std::dynamic_pointer_cast<LValue>(left);

    if (left->IsLValue())
    {
        if (const auto [signature, function] = FindFunction(op, left->GetType(), {right->GetType()}); function)
        {
            const auto cast_right = Cast(right, signature.Args[0]);
            if (!cast_right)
            {
                ThrowErrorStmt(expression, false, "Failed to cast: %s", cast_right.Msg().c_str());
                return nullptr;
            }

            const auto value = m_Builder->CreateCall(function->getFunctionType(), function, {lleft->GetPointer(), cast_right.Get()->GetValue()});
            return RValue::Create(signature.Result, value);
        }
    }

    if (op == "=")
    {
        if (!left->IsLValue())
        {
            ThrowErrorStmt(expression, false, "Cannot assign to rvalue");
            return nullptr;
        }

        const auto cast_right = Cast(right, lleft->GetType());
        if (!cast_right)
        {
            ThrowErrorStmt(expression, false, "Failed to cast: %s", cast_right.Msg().c_str());
            return nullptr;
        }

        lleft->StoreValue(cast_right.Get()->GetValue());
        return lleft->GetRValue();
    }

    const auto cast = CastToBestOf(left->GetRValue(), right->GetRValue());
    if (!cast)
    {
        ThrowErrorStmt(expression, false, "Failed to cast: %s", cast.Msg().c_str());
        return nullptr;
    }

    const auto& [lhs, rhs] = cast.Get();

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
            if (!left->IsLValue())
            {
                return nullptr;
            }
            const auto cast_right = Cast(value, lleft->GetType());
            if (!cast_right)
            {
                return nullptr;
            }
            lleft->StoreValue(cast_right.Get()->GetValue());
            return lleft->GetRValue();
        }
    }

    if (!value)
        ThrowErrorStmt(expression, false, "Binary operation '%s %s %s' is not implemented", left->GetType()->Name.c_str(), expression.Operator.c_str(), right->GetType()->Name.c_str());

    return value;
}
