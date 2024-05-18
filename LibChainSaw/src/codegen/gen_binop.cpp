#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Value.hpp>

csaw::RValuePtr csaw::Builder::GenCmpEQ(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpEQ(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFCmpOEQ(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenCmpNE(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpNE(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFCmpONE(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenCmpLE(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpSLE(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFCmpOLE(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenCmpGE(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpSGE(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFCmpOGE(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenAnd(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateAnd(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenLogicalAnd(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto value = m_Builder->CreateLogicalAnd(left->GetBoolValue(this), right->GetBoolValue(this));
    return RValue::Create(Type::GetInt1(), value);
}

csaw::RValuePtr csaw::Builder::GenOr(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateOr(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenLogicalOr(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto value = m_Builder->CreateLogicalOr(left->GetBoolValue(this), right->GetBoolValue(this));
    return RValue::Create(Type::GetInt1(), value);
}

csaw::RValuePtr csaw::Builder::GenXor(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateXor(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenCmpLT(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpSLT(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFCmpOLT(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenCmpGT(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpSGT(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFCmpOGT(left->GetValue(), right->GetValue());
        return RValue::Create(Type::GetInt1(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenShl(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateShl(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenAShr(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateAShr(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenLShr(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateLShr(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenAdd(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateAdd(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFAdd(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenSub(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateSub(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFSub(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenMul(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateMul(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFMul(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenDiv(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateSDiv(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFDiv(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    return nullptr;
}

csaw::RValuePtr csaw::Builder::GenRem(const RValuePtr& left, const RValuePtr& right) const
{
    if (!left || !right)
        return nullptr;

    const auto ty = Gen(left->GetType());

    if (ty->isIntegerTy())
    {
        const auto value = m_Builder->CreateSRem(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    if (ty->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFRem(left->GetValue(), right->GetValue());
        return RValue::Create(left->GetType(), value);
    }

    return nullptr;
}
