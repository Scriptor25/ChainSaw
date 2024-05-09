#include <iostream>
#include <map>
#include <string>
#include <csaw/Type.hpp>

static std::map<std::string, csaw::TypePtr> Types;

std::ostream& csaw::operator<<(std::ostream& out, const TypePtr& ptr)
{
    return out << ptr->Name;
}

csaw::TypePtr csaw::Type::Get(const std::string& name)
{
    auto& type = Types[name];
    if (type) return type;
    return type = std::make_shared<Type>(name);
}

void csaw::Type::Alias(const std::string& name, const TypePtr& origin)
{
    Types[name] = origin;
}

csaw::Type::Type(const std::string& name)
    : Name(name)
{
}

bool csaw::Type::IsPointer() const
{
    return false;
}

bool csaw::Type::IsStruct() const
{
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
    : Type(name), Base(base)
{
}

bool csaw::PointerType::IsPointer() const
{
    return true;
}

csaw::StructTypePtr csaw::StructType::Get(const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements)
{
    auto& type = Types[name];
    if (!type)
        type = std::make_shared<StructType>(name, elements);
    return std::dynamic_pointer_cast<StructType>(type);
}

csaw::StructTypePtr csaw::StructType::Get(const std::string& name)
{
    return std::dynamic_pointer_cast<StructType>(Types[name]);
}

csaw::StructType::StructType(const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements)
    : Type(name), Elements(elements)
{
}

bool csaw::StructType::IsStruct() const
{
    return true;
}
