#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Value.hpp>

csaw::LValuePtr csaw::LValue::Allocate(Builder* builder, const TypePtr& type)
{
    const auto pointer = builder->CreateAlloca(builder->Gen(type));
    return std::shared_ptr<LValue>(new LValue(builder, type, pointer));
}

csaw::LValuePtr csaw::LValue::AllocateAndStore(Builder* builder, const TypePtr& type, llvm::Value* value)
{
    const auto lvalue = Allocate(builder, type);
    lvalue->StoreValue(value);
    return lvalue;
}

csaw::LValuePtr csaw::LValue::Direct(Builder* builder, const TypePtr& type, llvm::Value* pointer)
{
    return std::shared_ptr<LValue>(new LValue(builder, type, pointer));
}

void csaw::LValue::StoreValue(llvm::Value* value) const
{
    m_Builder->GetBuilder().CreateStore(value, m_Pointer);
}

csaw::RValuePtr csaw::LValue::GetReference() const
{
    return RValue::Create(PointerType::Get(m_Type), m_Pointer);
}

csaw::LValuePtr csaw::LValue::Dereference() const
{
    if (!m_Type->IsPointer())
        CSAW_MESSAGE_NONE(true, "cannot dereference non-pointer lvalue");

    const auto type = m_Type->AsPointer()->Base;
    const auto pointer = GetValue();
    return std::shared_ptr<LValue>(new LValue(m_Builder, type, pointer));
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
    return m_Builder->GetBuilder().CreateLoad(m_Builder->Gen(m_Type), m_Pointer);
}

bool csaw::LValue::IsLValue() const
{
    return true;
}

csaw::RValuePtr csaw::LValue::GetRValue() const
{
    return RValue::Create(m_Type, GetValue());
}

csaw::LValue::LValue(Builder* builder, const TypePtr& type, llvm::Value* pointer)
    : m_Builder(builder), m_Type(type), m_Pointer(pointer)
{
}
