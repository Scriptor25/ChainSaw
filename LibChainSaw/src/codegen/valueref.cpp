#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/ValueRef.hpp>

csaw::ValueRef csaw::ValueRef::Allocate(Builder* builder, llvm::Value* value, const TypePtr& rawBaseType)
{
    if (!rawBaseType)
        CSAW_MESSAGE_NONE(true, "cannot allocate value for null base type");
    const auto insert_block = builder->GetBuilder().GetInsertBlock();
    builder->GetBuilder().SetInsertPointPastAllocas(insert_block->getParent());
    auto pointer = builder->GetBuilder().CreateAlloca(builder->Gen(rawBaseType));
    builder->GetBuilder().SetInsertPoint(insert_block);
    if (value)
        builder->GetBuilder().CreateStore(value, pointer);
    return {builder, false, pointer, PointerType::Get(rawBaseType)};
}

csaw::ValueRef csaw::ValueRef::Constant(Builder* builder, llvm::Value* value, const TypePtr& rawType)
{
    if (!rawType)
        CSAW_MESSAGE_NONE(true, "cannot create constant with null type");
    return {builder, true, value, rawType};
}

csaw::ValueRef csaw::ValueRef::Pointer(Builder* builder, llvm::Value* value, const TypePtr& rawBaseType)
{
    if (!rawBaseType)
        CSAW_MESSAGE_NONE(true, "cannot create value with null base type");
    return {builder, false, value, PointerType::Get(rawBaseType)};
}

llvm::Value* csaw::ValueRef::GetValue() const
{
    Check();
    return m_Value;
}

llvm::Type* csaw::ValueRef::GetType() const
{
    Check();
    return m_Builder->Gen(m_RawType);
}

llvm::Type* csaw::ValueRef::GetBaseType() const
{
    Check();
    return m_Builder->Gen(GetRawBaseType());
}

llvm::Type* csaw::ValueRef::GetBaseTypeBase() const
{
    Check();
    return m_Builder->Gen(GetRawBaseTypeBase());
}

csaw::TypePtr csaw::ValueRef::GetRawType() const
{
    Check();
    return m_RawType;
}

csaw::TypePtr csaw::ValueRef::GetRawBaseType() const
{
    Check();
    if (m_RawType->IsPointer())
        return std::dynamic_pointer_cast<PointerType>(m_RawType)->Base;
    return m_RawType;
}

csaw::TypePtr csaw::ValueRef::GetRawBaseTypeBase() const
{
    Check();
    auto type = GetRawBaseType();
    if (type->IsPointer())
        return std::dynamic_pointer_cast<PointerType>(type)->Base;
    return type;
}

csaw::ValueRef csaw::ValueRef::GetReference() const
{
    Check();
    if (m_IsRValue)
        CSAW_MESSAGE_NONE(true, "cannot get reference to rvalue");
    return Constant(m_Builder, m_Value, m_RawType);
}

bool csaw::ValueRef::IsRValue() const
{
    return m_IsRValue;
}

csaw::ValueRef csaw::ValueRef::Load() const
{
    Check();
    if (m_IsRValue)
        return *this;
    const auto base_type = GetRawBaseType();
    const auto value = m_Builder->GetBuilder().CreateLoad(m_Builder->Gen(base_type), m_Value);
    return Constant(m_Builder, value, base_type);
}

const csaw::ValueRef& csaw::ValueRef::Store(const ValueRef& value) const
{
    Check();
    if (m_IsRValue)
        CSAW_MESSAGE_NONE(true, "cannot assign to rvalue");
    m_Builder->GetBuilder().CreateStore(value.Load().GetValue(), m_Value);
    return *this;
}

bool csaw::ValueRef::Invalid() const
{
    return !m_Builder || !m_Value || !m_RawType;
}

csaw::ValueRef::ValueRef()
    : m_Builder(nullptr), m_IsRValue(true), m_Value(nullptr)
{
}

csaw::ValueRef::ValueRef(Builder* builder, const bool rvalue, llvm::Value* value, const TypePtr& rawType)
    : m_Builder(builder), m_IsRValue(rvalue), m_Value(value), m_RawType(rawType)
{
}

void csaw::ValueRef::Check() const
{
    if (Invalid())
        CSAW_MESSAGE_NONE(true, "cannot use invalid value reference");
}
