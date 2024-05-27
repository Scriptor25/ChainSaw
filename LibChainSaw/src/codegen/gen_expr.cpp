#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::ValuePtr csaw::Builder::Gen(const ExpressionPtr& ptr)
{
    if (!ptr)
        return nullptr;

    if (const auto p = std::dynamic_pointer_cast<BinaryExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<CallExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<CastExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<CharExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<DereferenceExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<FloatExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<IdentifierExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<IndexExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<IntExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<MemberExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<ReferenceExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<SelectExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<SizeOfExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<StringExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<UnaryExpression>(ptr))
        return Gen(*p);

    ThrowErrorStmt(*ptr, false, "Code generation is not implemented for this expression");
    return nullptr;
}

csaw::RValuePtr csaw::Builder::Gen(const CastExpression& expression)
{
    const auto value = Gen(expression.Value);
    if (!value)
        return nullptr;

    const auto cast = Cast(value, expression.Type);
    if (!cast)
    {
        ThrowErrorStmt(expression, false, "Failed to cast: %s", cast.Msg().c_str());
        return nullptr;
    }

    return cast.Get();
}

csaw::RValuePtr csaw::Builder::Gen(const CharExpression& expression) const
{
    const auto value = GetBuilder().getInt8(expression.Value);
    return RValue::Create(Type::GetInt8(), value);
}

csaw::LValuePtr csaw::Builder::Gen(const DereferenceExpression& expression)
{
    const auto value = Gen(expression.Value);
    if (!value)
        return nullptr;

    if (!value->IsLValue())
    {
        const auto deref = value->GetRValue()->Dereference(this);
        if (!deref)
        {
            ThrowErrorStmt(expression, false, "Failed to dereference: %s", deref.Msg().c_str());
            return nullptr;
        }
        return deref.Get();
    }

    const auto lvalue = std::dynamic_pointer_cast<LValue>(value);
    const auto deref = lvalue->Dereference();
    if (!deref)
    {
        ThrowErrorStmt(expression, false, "Failed to dereference: %s", deref.Msg().c_str());
        return nullptr;
    }

    return deref.Get();
}

csaw::RValuePtr csaw::Builder::Gen(const FloatExpression& expression) const
{
    const auto value = llvm::ConstantFP::get(GetBuilder().getDoubleTy(), expression.Value);
    return RValue::Create(Type::GetFlt64(), value);
}

csaw::ValuePtr csaw::Builder::Gen(const IdentifierExpression& expression)
{
    if (const auto& value = m_Values[expression.Id])
        return value;

    if (const auto value = GetModule().getFunction(expression.Id))
    {
        const auto type = m_Signatures[value].GetFunctionType();
        return LValue::Direct(this, type, value);
    }

    ThrowErrorStmt(expression, false, "Undefined identifier '%s'", expression.Id.c_str());
    return nullptr;
}

csaw::RValuePtr csaw::Builder::Gen(const IntExpression& expression) const
{
    const auto value = GetBuilder().getInt64(expression.Value);
    return RValue::Create(Type::GetInt64(), value);
}

csaw::RValuePtr csaw::Builder::Gen(const ReferenceExpression& expression)
{
    const auto value = Gen(expression.Value);
    if (!value)
        return nullptr;

    if (!value->IsLValue())
    {
        ThrowErrorStmt(expression, false, "Cannot get reference to rvalue");
        return nullptr;
    }

    return std::dynamic_pointer_cast<LValue>(value)->GetReference();
}

csaw::RValuePtr csaw::Builder::Gen(const SizeOfExpression& expression) const
{
    const auto type = Gen(expression.Type);
    if (!type)
    {
        ThrowErrorStmt(expression, false, "Failed to generate type %s: %s", expression.Type->Name, type.Msg().c_str());
        return nullptr;
    }

    const auto size = GetModule().getDataLayout().getTypeAllocSize(type.Get());
    return RValue::Create(Type::GetInt64(), GetBuilder().getInt64(size));
}

csaw::RValuePtr csaw::Builder::Gen(const StringExpression& expression) const
{
    const auto value = GetBuilder().CreateGlobalStringPtr(expression.Value);
    return RValue::Create(PointerType::Get(Type::GetInt8()), value);
}
