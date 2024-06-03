#include <csaw/Builder.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::Builder::GenCmpEQ(const RValuePtr& left, const RValuePtr& right)
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

csaw::RValuePtr csaw::Builder::GenCmpNE(const RValuePtr& left, const RValuePtr& right)
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

csaw::RValuePtr csaw::Builder::GenCmpLE(const RValuePtr& left, const RValuePtr& right)
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

csaw::RValuePtr csaw::Builder::GenCmpGE(const RValuePtr& left, const RValuePtr& right)
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

csaw::RValuePtr csaw::Builder::GenAnd(const RValuePtr& left, const RValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateAnd(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenLogicalAnd(const RValuePtr& left, const RValuePtr& right)
{
    const auto value = GetBuilder().CreateLogicalAnd(left->GetBoolValue(), right->GetBoolValue());
    return RValue::Create(this, Type::GetInt1(), value);
}

csaw::RValuePtr csaw::Builder::GenOr(const RValuePtr& left, const RValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateOr(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenLogicalOr(const RValuePtr& left, const RValuePtr& right)
{
    const auto value = GetBuilder().CreateLogicalOr(left->GetBoolValue(), right->GetBoolValue());
    return RValue::Create(this, Type::GetInt1(), value);
}

csaw::RValuePtr csaw::Builder::GenXor(const RValuePtr& left, const RValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateXor(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenCmpLT(const RValuePtr& left, const RValuePtr& right)
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

csaw::RValuePtr csaw::Builder::GenCmpGT(const RValuePtr& left, const RValuePtr& right)
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

csaw::RValuePtr csaw::Builder::GenShl(const RValuePtr& left, const RValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateShl(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenAShr(const RValuePtr& left, const RValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateAShr(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenLShr(const RValuePtr& left, const RValuePtr& right)
{
    const auto ty = Gen(left->GetType()).Get();

    if (ty->isIntegerTy())
    {
        const auto value = GetBuilder().CreateLShr(left->GetValue(), right->GetValue());
        return RValue::Create(this, left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenAdd(const RValuePtr& left, const RValuePtr& right)
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

csaw::RValuePtr csaw::Builder::GenSub(const RValuePtr& left, const RValuePtr& right)
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

csaw::RValuePtr csaw::Builder::GenMul(const RValuePtr& left, const RValuePtr& right)
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

csaw::RValuePtr csaw::Builder::GenDiv(const RValuePtr& left, const RValuePtr& right)
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

csaw::RValuePtr csaw::Builder::GenRem(const RValuePtr& left, const RValuePtr& right)
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
