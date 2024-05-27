#include <csaw/Builder.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::RValue::Create(const TypePtr& type, llvm::Value* value)
{
    return std::shared_ptr<RValue>(new RValue(type, value));
}

csaw::Expect<csaw::LValuePtr> csaw::RValue::Dereference(Builder* builder) const
{
    if (!m_Type->IsPointer())
        return Expect<LValuePtr>("Value type is non-pointer");

    const auto type = m_Type->AsPointer()->Base;
    const auto pointer = m_Value;
    return LValue::Direct(builder, type, pointer);
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
    return Create(m_Type, m_Value);
}

csaw::RValue::RValue(const TypePtr& type, llvm::Value* value)
    : m_Type(type), m_Value(value)
{
}
