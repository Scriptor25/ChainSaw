#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/ValueRef.hpp>

csaw::ValueRef csaw::Builder::GenCmpEQ(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpEQ(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenCmpNE(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpNE(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenCmpLE(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpSLE(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    if (left.GetType()->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFCmpOLE(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenCmpGE(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpSGE(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    if (left.GetType()->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFCmpOGE(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenAnd(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateAnd(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenLogicalAnd(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy(1))
    {
        const auto value = m_Builder->CreateLogicalAnd(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenOr(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateOr(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenLogicalOr(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy(1))
    {
        const auto value = m_Builder->CreateLogicalOr(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenXor(const ValueRef& left, const ValueRef& right)
{
    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenCmpLT(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpSLT(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    if (left.GetType()->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFCmpOLT(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenCmpGT(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateICmpSGT(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    if (left.GetType()->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFCmpOGT(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, Type::Get("int1"));
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenShl(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateShl(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenAShr(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateAShr(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenLShr(const ValueRef& left, const ValueRef& right)
{
    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenAdd(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateAdd(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    if (left.GetType()->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFAdd(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenSub(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateSub(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    if (left.GetType()->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFSub(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenMul(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateMul(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    if (left.GetType()->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFMul(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenDiv(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateSDiv(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    if (left.GetType()->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFDiv(left.GetValue(), right.GetValue());
        return ValueRef::Constant(this, value, left.GetRawType());
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenRem(const ValueRef& left, const ValueRef& right)
{
    CSAW_WIP;
}
