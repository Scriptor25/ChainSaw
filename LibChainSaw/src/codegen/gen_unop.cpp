#include <csaw/codegen/Builder.hpp>

csaw::ValueRef csaw::Builder::GenNeg(const ValueRef& reference)
{
    if (reference.GetBaseType()->isIntegerTy())
    {
        const auto value = m_Builder->CreateNeg(reference.Load().GetValue());
        return ValueRef::Constant(this, value, reference.GetRawBaseType());
    }

    if (reference.GetBaseType()->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFNeg(reference.Load().GetValue());
        return ValueRef::Constant(this, value, reference.GetRawBaseType());
    }

    return {};
}

csaw::ValueRef csaw::Builder::GenNot(const ValueRef& reference)
{
    if (reference.GetBaseType()->isIntegerTy(1))
    {
        const auto value = m_Builder->CreateNot(reference.Load().GetValue());
        return ValueRef::Constant(this, value, Type::GetInt1());
    }

    return {};
}

csaw::ValueRef csaw::Builder::GenInv(const ValueRef& reference)
{
    return {};
}

csaw::ValueRef csaw::Builder::GenInc(const ValueRef& reference, const bool opRight)
{
    const auto base_type = reference.GetBaseType();
    if (base_type->isIntegerTy())
    {
        const auto one = llvm::ConstantInt::get(base_type, 1, true);
        const auto value = m_Builder->CreateAdd(reference.Load().GetValue(), one);
        auto result = ValueRef::Constant(this, value, reference.GetRawBaseType());
        (void)reference.Store(result);
        if (opRight)
            return result;
        return reference;
    }

    return {};
}

csaw::ValueRef csaw::Builder::GenDec(const ValueRef& reference, const bool opRight)
{
    return {};
}
