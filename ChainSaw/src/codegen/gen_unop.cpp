#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>

csaw::ValueRef csaw::Builder::GenNeg(const ValueRef& reference)
{
    if (reference.GetBaseType()->isFloatingPointTy())
    {
        const auto value = m_Builder->CreateFNeg(reference.Load().GetValue());
        return ValueRef::Constant(this, value, reference.GetRawBaseType());
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenNot(const ValueRef& reference)
{
    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenInv(const ValueRef& reference)
{
    CSAW_WIP;
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

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::GenDec(const ValueRef& reference, const bool opRight)
{
    CSAW_WIP;
}
