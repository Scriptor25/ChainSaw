#include <csaw/Builder.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::Expect<csaw::LValuePtr> csaw::LValue::Allocate(Builder* builder, const TypePtr& type)
{
    const auto ty = builder->Gen(type);
    if (!ty)
        return Expect<LValuePtr>("Type is null: " + ty.Msg());

    const auto pointer = builder->CreateAlloca(ty.Get());
    if (!pointer)
        return Expect<LValuePtr>("Type " + type->Name + " is not defined");

    return std::shared_ptr<LValue>(new LValue(builder, type, pointer));
}

csaw::Expect<csaw::LValuePtr> csaw::LValue::AllocateAndStore(Builder* builder, const TypePtr& type, const ValuePtr& value)
{
    auto lvalue = Allocate(builder, type);
    if (!lvalue)
        return Expect<LValuePtr>("Failed to allocate: " + lvalue.Msg());

    if (const auto store = lvalue.Get()->StoreValue(value); !store)
        return Expect<LValuePtr>("Failed to store: " + store.Msg());

    return lvalue;
}

csaw::LValuePtr csaw::LValue::Direct(Builder* builder, const TypePtr& type, llvm::Value* pointer)
{
    return std::shared_ptr<LValue>(new LValue(builder, type, pointer));
}

csaw::Expect<csaw::RValuePtr> csaw::LValue::StoreValue(const ValuePtr& value) const
{
    if (!value)
    {
        const auto type = m_Builder->Gen(m_Type);
        if (!type)
            return Expect<RValuePtr>("Failed to generate type " + m_Type->Name + ": " + type.Msg());
        const auto null_value = llvm::Constant::getNullValue(type.Get());
        m_Builder->GetBuilder().CreateStore(null_value, m_Pointer);
        return RValue::Create(m_Builder, m_Type, null_value);
    }

    auto cast = m_Builder->Cast(value, m_Type);
    if (!cast)
        return Expect<RValuePtr>("Failed to cast: " + cast.Msg());
    m_Builder->GetBuilder().CreateStore(cast.Get()->GetValue(), m_Pointer);
    return cast;
}

csaw::RValuePtr csaw::LValue::GetReference() const
{
    return RValue::Create(m_Builder, PointerType::Get(m_Type), m_Pointer);
}

llvm::Value* csaw::LValue::GetPointer() const
{
    return m_Pointer;
}

csaw::TypePtr csaw::LValue::GetType() const
{
    return m_Type;
}

llvm::Value* csaw::LValue::GetValue() const
{
    const auto type = m_Builder->Gen(m_Type);
    return m_Builder->GetBuilder().CreateLoad(type.Get(), m_Pointer);
}

bool csaw::LValue::IsLValue() const
{
    return true;
}

csaw::RValuePtr csaw::LValue::GetRValue() const
{
    return RValue::Create(m_Builder, m_Type, GetValue());
}

csaw::Expect<csaw::LValuePtr> csaw::LValue::Dereference() const
{
    if (!m_Type->IsPointer())
        return Expect<LValuePtr>("Value type is non-pointer");

    const auto type = m_Type->AsPointer()->Base;
    const auto pointer = GetValue();
    return std::shared_ptr<LValue>(new LValue(m_Builder, type, pointer));
}

llvm::Value* csaw::LValue::GetBoolValue() const
{
    return m_Builder->GetBuilder().CreateIsNotNull(GetValue());
}

csaw::LValue::LValue(Builder* builder, const TypePtr& type, llvm::Value* pointer)
    : m_Builder(builder), m_Type(type), m_Pointer(pointer)
{
}
