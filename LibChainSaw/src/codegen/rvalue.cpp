#include <csaw/Builder.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::RValue::Create(Builder* builder, const TypePtr& type, llvm::Value* value)
{
    return std::shared_ptr<RValue>(new RValue(builder, type, value));
}

csaw::TypePtr csaw::RValue::GetType() const
{
    return m_Type;
}

llvm::Value* csaw::RValue::GetValue() const
{
    return m_Value;
}

bool csaw::RValue::IsLValue() const
{
    return false;
}

csaw::RValuePtr csaw::RValue::GetRValue() const
{
    return Create(m_Builder, m_Type, m_Value);
}

csaw::Expect<csaw::LValuePtr> csaw::RValue::Dereference() const
{
    if (!m_Type->IsPointer())
        return Expect<LValuePtr>("Value type is non-pointer");

    const auto type = m_Type->AsPointer()->Base;
    const auto pointer = m_Value;
    return LValue::Direct(m_Builder, type, pointer);
}

llvm::Value* csaw::RValue::GetBoolValue() const
{
    return m_Builder->GetBuilder().CreateIsNotNull(m_Value);
}

csaw::RValue::RValue(Builder* builder, const TypePtr& type, llvm::Value* value)
    : m_Builder(builder), m_Type(type), m_Value(value)
{
}
