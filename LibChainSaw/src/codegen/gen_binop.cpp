#include <csaw/Builder.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::ValuePtr csaw::Builder::GenCmpEQ(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateICmpEQ(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = GetBuilder().CreateFCmpOEQ(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenCmpNE(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateICmpNE(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = GetBuilder().CreateFCmpONE(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenCmpLE(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateICmpSLE(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = GetBuilder().CreateFCmpOLE(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenCmpGE(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateICmpSGE(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = GetBuilder().CreateFCmpOGE(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenAnd(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateAnd(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenLogicalAnd(const ValuePtr& left, const ValuePtr& right)
{
    const auto value = GetBuilder().CreateLogicalAnd(left->GetBoolValue(), right->GetBoolValue());
    return RValue::Create(this, Type::GetInt1(), value);
}

csaw::ValuePtr csaw::Builder::GenOr(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateOr(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenLogicalOr(const ValuePtr& left, const ValuePtr& right)
{
    const auto value = GetBuilder().CreateLogicalOr(left->GetBoolValue(), right->GetBoolValue());
    return RValue::Create(this, Type::GetInt1(), value);
}

csaw::ValuePtr csaw::Builder::GenXor(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateXor(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenCmpLT(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateICmpSLT(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = GetBuilder().CreateFCmpOLT(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenCmpGT(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateICmpSGT(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = GetBuilder().CreateFCmpOGT(left->GetValue(), right->GetValue());
        return RValue::Create(this, Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenShl(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateShl(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenAShr(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateAShr(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenLShr(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateLShr(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenAdd(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateAdd(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = GetBuilder().CreateFAdd(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenSub(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateSub(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = GetBuilder().CreateFSub(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenMul(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateMul(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = GetBuilder().CreateFMul(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenDiv(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateSDiv(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = GetBuilder().CreateFDiv(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::ValuePtr csaw::Builder::GenRem(const ValuePtr& left, const ValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateSRem(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = GetBuilder().CreateFRem(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}
