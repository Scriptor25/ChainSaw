#include <csaw/codegen/Builder.hpp>

csaw::ValueRef csaw::Builder::GenNeg(const ValueRef& value)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenNot(const ValueRef& value)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenInv(const ValueRef& value)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenInc(const ValueRef& value, const bool opRight)
{
    if (value.Type()->isIntegerTy())
    {
        const auto result = m_Builder->CreateAdd(value.Load(*this), llvm::ConstantInt::get(value.Type(), 1));

        value.Store(*this, result);
        if (opRight)
            return {*this, ValueRefMode_Constant, result, value.RawType()};
        return value;
    }

    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::GenDec(const ValueRef& value, const bool opRight)
{
    throw std::runtime_error("not yet implemented");
}
