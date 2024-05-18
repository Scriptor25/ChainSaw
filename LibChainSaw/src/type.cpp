#include <iostream>
#include <map>
#include <csaw/Message.hpp>
#include <csaw/Type.hpp>

static std::map<std::string, csaw::TypePtr> Types = {
    {"void", std::make_shared<csaw::Type>("void", false, 0)},
    {"int1", std::make_shared<csaw::Type>("int1", false, 1)},
    {"int8", std::make_shared<csaw::Type>("int8", false, 8)},
    {"int16", std::make_shared<csaw::Type>("int16", false, 16)},
    {"int32", std::make_shared<csaw::Type>("int32", false, 32)},
    {"int64", std::make_shared<csaw::Type>("int64", false, 64)},
    {"int128", std::make_shared<csaw::Type>("int128", false, 128)},
    {"flt16", std::make_shared<csaw::Type>("flt16", true, 16)},
    {"flt32", std::make_shared<csaw::Type>("flt32", true, 32)},
    {"flt64", std::make_shared<csaw::Type>("flt64", true, 64)},
};

std::ostream& csaw::operator<<(std::ostream& out, const TypePtr& ptr)
{
    return out << ptr->Name;
}

csaw::TypePtr csaw::Type::Get(const std::string& name)
{
    if (const auto& type = Types[name])
        return type;
    return nullptr;
}

csaw::TypePtr csaw::Type::GetVoid()
{
    return Types["void"];
}

csaw::TypePtr csaw::Type::GetInt1()
{
    return Types["int1"];
}

csaw::TypePtr csaw::Type::GetInt8()
{
    return Types["int8"];
}

csaw::TypePtr csaw::Type::GetInt16()
{
    return Types["int16"];
}

csaw::TypePtr csaw::Type::GetInt32()
{
    return Types["int32"];
}

csaw::TypePtr csaw::Type::GetInt64()
{
    return Types["int64"];
}

csaw::TypePtr csaw::Type::GetInt128()
{
    return Types["int128"];
}

csaw::TypePtr csaw::Type::GetFlt16()
{
    return Types["flt16"];
}

csaw::TypePtr csaw::Type::GetFlt32()
{
    return Types["flt32"];
}

csaw::TypePtr csaw::Type::GetFlt64()
{
    return Types["flt64"];
}

csaw::TypePtr csaw::Type::GetOrCreate(const std::string& name)
{
    auto& type = Types[name];
    if (type)
        return type;
    return type = std::make_shared<Type>(name, false, 0);
}

void csaw::Type::Alias(const std::string& name, const TypePtr& origin)
{
    Types[name] = origin;
}

csaw::Type::Type(const std::string& name, const bool is_flt, const size_t bits)
    : Name(name), IsFlt(is_flt), Bits(bits)
{
}

bool csaw::Type::IsPointer() const
{
    return false;
}

bool csaw::Type::IsArray() const
{
    return false;
}

bool csaw::Type::IsStruct() const
{
    return false;
}

bool csaw::Type::IsFunction() const
{
    return false;
}

const csaw::PointerType* csaw::Type::AsPointer() const
{
    return dynamic_cast<const PointerType*>(this);
}

const csaw::ArrayType* csaw::Type::AsArray() const
{
    return dynamic_cast<const ArrayType*>(this);
}

const csaw::StructType* csaw::Type::AsStruct() const
{
    return dynamic_cast<const StructType*>(this);
}

const csaw::FunctionType* csaw::Type::AsFunction() const
{
    return dynamic_cast<const FunctionType*>(this);
}

bool csaw::Type::ParentOf(const TypePtr& type) const
{
    if (this == type.get() || this == GetVoid().get())
        return true;

    if (IsStruct() || type->IsStruct() || IsFunction())
        return false;

    if (IsPointer())
        return type->IsPointer() ? AsPointer()->Base->ParentOf(type->AsPointer()->Base) : type->IsFunction() ? AsPointer()->Base->ParentOf(type) : false;

    if (Bits >= type->Bits && (IsFlt || !type->IsFlt))
        return true;

    return false;
}

csaw::PointerTypePtr csaw::PointerType::Get(const TypePtr& base)
{
    const auto name = base->Name + '*';
    auto& type = Types[name];
    if (!type)
        type = std::make_shared<PointerType>(name, base);
    return std::dynamic_pointer_cast<PointerType>(type);
}

csaw::PointerType::PointerType(const std::string& name, const TypePtr& base)
    : Type(name, false, 0), Base(base)
{
}

bool csaw::PointerType::IsPointer() const
{
    return true;
}

csaw::ArrayTypePtr csaw::ArrayType::Get(const TypePtr& base, const size_t size)
{
    const auto name = base->Name + '[' + std::to_string(size) + ']';
    auto& type = Types[name];
    if (!type)
        type = std::make_shared<ArrayType>(name, base, size);
    return std::dynamic_pointer_cast<ArrayType>(type);
}

csaw::ArrayType::ArrayType(const std::string& name, const TypePtr& base, const size_t size)
    : Type(name, false, 0), Base(base), Size(size)
{
}

bool csaw::ArrayType::IsArray() const
{
    return true;
}

csaw::StructTypePtr csaw::StructType::Create(const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements)
{
    auto& type = Types[name];
    if (!type)
        type = std::make_shared<StructType>(name, elements);
    const auto& struct_type = std::dynamic_pointer_cast<StructType>(type);
    struct_type->Elements = elements;
    return struct_type;
}

csaw::StructTypePtr csaw::StructType::Get(const std::string& name)
{
    return std::dynamic_pointer_cast<StructType>(Types[name]);
}

csaw::StructType::StructType(const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements)
    : Type(name, false, 0), Elements(elements)
{
}

bool csaw::StructType::IsStruct() const
{
    return true;
}

std::pair<int, csaw::TypePtr> csaw::StructType::GetElement(const std::string& name) const
{
    for (size_t i = 0; i < Elements.size(); ++i)
    {
        const auto& [ename, etype] = Elements[i];
        if (ename == name)
            return {static_cast<int>(i), etype};
    }

    return {-1, {}};
}

csaw::FunctionTypePtr csaw::FunctionType::Get(const TypePtr& result, const std::vector<TypePtr>& args, const bool is_vararg)
{
    std::string name = '%' + result->Name + '(';
    for (size_t i = 0; i < args.size(); ++i)
    {
        if (i > 0) name += ", ";
        name += args[i]->Name;
    }
    name += ')';

    auto& type = Types[name];
    if (!type)
        type = std::make_shared<FunctionType>(name, result, args, is_vararg);
    return std::dynamic_pointer_cast<FunctionType>(type);
}

csaw::FunctionType::FunctionType(const std::string& name, const TypePtr& result, const std::vector<TypePtr>& args, const bool is_vararg)
    : Type(name, false, 0), Result(result), Args(args), IsVararg(is_vararg)
{
}

bool csaw::FunctionType::IsFunction() const
{
    return true;
}
