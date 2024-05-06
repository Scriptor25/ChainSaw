#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/ValueRef.hpp>

csaw::ValueRef csaw::Builder::GenCmpEQ(const ValueRef& left, const ValueRef& right)
{
    if (left.Type()->isIntegerTy() && right.Type()->isIntegerTy())
    {
        const auto result = m_Builder->CreateICmpEQ(left.Load(*this), right.Load(*this));
        return {*this, ValueRefMode_Constant, result, Type::Get("int1")};
    }

    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenCmpNE(const ValueRef& left, const ValueRef& right)
{
    if (left.Type()->isIntOrPtrTy() && right.Type()->isIntOrPtrTy())
    {
        const auto result = m_Builder->CreateICmpNE(left.Load(*this), right.Load(*this));
        return {*this, ValueRefMode_Constant, result, Type::Get("int1")};
    }

    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenCmpLE(const ValueRef& left, const ValueRef& right)
{
    if (left.Type()->isIntegerTy() && right.Type()->isIntegerTy())
    {
        const auto result = m_Builder->CreateICmpSLE(left.Load(*this), right.Load(*this));
        return {*this, ValueRefMode_Constant, result, Type::Get("int1")};
    }

    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenCmpGE(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenAnd(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenLogicalAnd(const ValueRef& left, const ValueRef& right)
{
    if (left.Type()->isIntegerTy(1) && right.Type()->isIntegerTy(1))
    {
        const auto result = m_Builder->CreateLogicalAnd(left.Load(*this), right.Load(*this));
        return {*this, ValueRefMode_Constant, result, Type::Get("int1")};
    }

    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenOr(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenLogicalOr(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenXor(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenCmpLT(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenCmpGT(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenShl(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenAShr(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenLShr(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenAdd(const ValueRef& left, const ValueRef& right)
{
    if (left.Type()->isIntegerTy() && right.Type()->isIntegerTy())
    {
        const auto result = m_Builder->CreateAdd(left.Load(*this), right.Load(*this));
        return {*this, ValueRefMode_Constant, result, left.RawType()};
    }

    if (left.Type()->isFloatingPointTy() && right.Type()->isFloatingPointTy())
    {
        const auto result = m_Builder->CreateFAdd(left.Load(*this), right.Load(*this));
        return {*this, ValueRefMode_Constant, result, left.RawType()};
    }

    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenSub(const ValueRef& left, const ValueRef& right)
{
    if (left.Type()->isIntegerTy() && right.Type()->isIntegerTy())
    {
        const auto result = m_Builder->CreateSub(left.Load(*this), right.Load(*this));
        return {*this, ValueRefMode_Constant, result, left.RawType()};
    }

    if (left.Type()->isFloatingPointTy() && right.Type()->isFloatingPointTy())
    {
        const auto result = m_Builder->CreateFSub(left.Load(*this), right.Load(*this));
        return {*this, ValueRefMode_Constant, result, left.RawType()};
    }

    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenMul(const ValueRef& left, const ValueRef& right)
{
    if (left.Type()->isFloatingPointTy() && right.Type()->isFloatingPointTy())
    {
        const auto result = m_Builder->CreateFMul(left.Load(*this), right.Load(*this));
        return {*this, ValueRefMode_Constant, result, left.RawType()};
    }

    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenDiv(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenRem(const ValueRef& left, const ValueRef& right)
{
    throw std::runtime_error("not yet implemented");
}
