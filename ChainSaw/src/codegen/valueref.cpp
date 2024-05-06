#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/ValueRef.hpp>

csaw::ValueRef::ValueRef()
{
    m_Mode = ValueRefMode_Invalid;
    m_RawType = nullptr;
    m_Type = nullptr;
    m_Pointer = nullptr;
}

csaw::ValueRef::ValueRef(Builder& builder, const ValueRefMode mode, llvm::Value* value, const TypePtr& rawType)
    : m_Mode(mode), m_RawType(rawType), m_Type(builder.Gen(rawType))
{
    if (!value)
        value = llvm::Constant::getNullValue(m_Type);

    switch (mode)
    {
    case ValueRefMode_Constant:
    case ValueRefMode_Pointer:
        m_Pointer = value;
        break;

    case ValueRefMode_AllocateValue:
        m_Pointer = builder.GetBuilder().CreateAlloca(m_Type);
        Store(builder, value);
        break;

    default:
        throw std::runtime_error("invalid value reference");
    }
}

csaw::TypePtr csaw::ValueRef::RawType() const
{
    return m_RawType;
}

llvm::Type* csaw::ValueRef::Type() const
{
    return m_Type;
}

csaw::ValueRefMode csaw::ValueRef::Mode() const
{
    return m_Mode;
}

llvm::Value* csaw::ValueRef::Load(Builder& builder) const
{
    switch (m_Mode)
    {
    case ValueRefMode_Constant:
        return m_Pointer;

    case ValueRefMode_Pointer:
    case ValueRefMode_AllocateValue:
        return builder.GetBuilder().CreateLoad(m_Type, m_Pointer);

    default:
        throw std::runtime_error("invalid value reference");
    }
}

llvm::Value* csaw::ValueRef::Store(Builder& builder, llvm::Value* value) const
{
    switch (m_Mode)
    {
    case ValueRefMode_Pointer:
    case ValueRefMode_AllocateValue:
        return builder.GetBuilder().CreateStore(value, m_Pointer);

    default:
        throw std::runtime_error("invalid or constant value reference");
    }
}
