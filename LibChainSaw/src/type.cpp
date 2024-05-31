#include <iostream>
#include <map>
#include <csaw/Type.hpp>

#define INFO_NONE    0b00000000
#define INFO_IS_VOID 0b00000001
#define INFO_IS_INT  0b00000010
#define INFO_IS_FLT  0b00000100
#define INFO_IS_PTR  0b00001000
#define INFO_IS_ARR  0b00010000
#define INFO_IS_STR  0b00100000
#define INFO_IS_FUN  0b01000000
#define INFO_IS_TEMP 0b10000000

static std::map<std::string, csaw::TypePtr> Types = {
    {"void", std::make_shared<csaw::Type>("void", INFO_IS_VOID)},
    {"int1", std::make_shared<csaw::Type>("int1", INFO_IS_INT)},
    {"int8", std::make_shared<csaw::Type>("int8", INFO_IS_INT)},
    {"int16", std::make_shared<csaw::Type>("int16", INFO_IS_INT)},
    {"int32", std::make_shared<csaw::Type>("int32", INFO_IS_INT)},
    {"int64", std::make_shared<csaw::Type>("int64", INFO_IS_INT)},
    {"int128", std::make_shared<csaw::Type>("int128", INFO_IS_INT)},
    {"flt16", std::make_shared<csaw::Type>("flt16", INFO_IS_FLT)},
    {"flt32", std::make_shared<csaw::Type>("flt32", INFO_IS_FLT)},
    {"flt64", std::make_shared<csaw::Type>("flt64", INFO_IS_FLT)},
};

std::ostream& csaw::operator<<(std::ostream& out, const TypePtr& ptr)
{
    return out << ptr->Name;
}

std::ostream& csaw::operator<<(std::ostream& out, const Arg& arg)
{
    if (arg.Name.empty())
        return out << arg.Type;
    return out << arg.Name << ": " << arg.Type;
}

csaw::TypePtr csaw::Type::Get(const std::string& name)
{
    auto& type = Types[name];
    if (!type) type = std::make_shared<Type>(name, INFO_NONE);
    return type;
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

void csaw::Type::Alias(const std::string& name, const TypePtr& origin)
{
    Types[name] = origin;
}

csaw::Type::Type(const std::string& name, const int info)
    : Name(name), Info(info)
{
}

bool csaw::Type::IsVoid() const
{
    return Info & INFO_IS_VOID;
}

bool csaw::Type::IsInt() const
{
    return Info & INFO_IS_INT;
}

bool csaw::Type::IsFlt() const
{
    return Info & INFO_IS_FLT;
}

bool csaw::Type::IsPointer() const
{
    return Info & INFO_IS_PTR;
}

bool csaw::Type::IsArray() const
{
    return Info & INFO_IS_ARR;
}

bool csaw::Type::IsStruct() const
{
    return Info & INFO_IS_STR;
}

bool csaw::Type::IsFunction() const
{
    return Info & INFO_IS_FUN;
}

bool csaw::Type::IsTemplate() const
{
    return Info & INFO_IS_TEMP;
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

const csaw::TemplateType* csaw::Type::AsTemplate() const
{
    return dynamic_cast<const TemplateType*>(this);
}

bool csaw::Type::ParentOf(const TypePtr& type) const
{
    if (this == type.get())
        return true;

    if (IsVoid())
        return true;

    if (IsPointer())
    {
        const auto ptr = AsPointer();

        if (type->IsPointer())
            return ptr->Base->ParentOf(type->AsPointer()->Base);

        return false;
    }

    if (IsArray() && type->IsArray())
    {
        const auto aarr = AsArray();
        const auto barr = type->AsArray();

        if (aarr->Size != barr->Size)
            return false;

        return aarr->Base->ParentOf(barr->Base);
    }

    if (IsFunction() && type->IsFunction())
    {
        const auto afunc = AsFunction();
        const auto bfunc = type->AsFunction();

        if (!afunc->Result->IsVoid() && afunc->Result != bfunc->Result)
            return false;

        if (afunc->IsVararg && !bfunc->IsVararg)
            return false;

        if (bfunc->Args.size() > afunc->Args.size())
            return false;

        for (size_t i = 0; i < bfunc->Args.size(); ++i)
            if (!bfunc->Args[i]->ParentOf(afunc->Args[i]))
                return false;

        return true;
    }

    if ((IsInt() || IsFlt()) && (type->IsInt() || type->IsFlt()))
        return IsFlt() || !type->IsFlt();

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
    : Type(name, INFO_IS_PTR), Base(base)
{
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
    : Type(name, INFO_IS_ARR), Base(base), Size(size)
{
}

csaw::StructTypePtr csaw::StructType::Create(const std::string& name, const std::vector<Arg>& elements)
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

csaw::StructType::StructType(const std::string& name, const std::vector<Arg>& elements)
    : Type(name, INFO_IS_STR), Elements(elements)
{
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

csaw::FunctionTypePtr csaw::FunctionType::Get(const std::vector<TypePtr>& args, const bool is_vararg, const TypePtr& result)
{
    std::string name = "(";
    for (size_t i = 0; i < args.size(); ++i)
    {
        if (i > 0) name += ", ";
        name += args[i]->Name;
    }
    name += ")(" + result->Name + ')';

    auto& type = Types[name];
    if (!type)
        type = std::make_shared<FunctionType>(name, args, is_vararg, result);
    return std::dynamic_pointer_cast<FunctionType>(type);
}

csaw::FunctionType::FunctionType(const std::string& name, const std::vector<TypePtr>& args, const bool is_vararg, const TypePtr& result)
    : Type(name, INFO_IS_FUN), Args(args), IsVararg(is_vararg), Result(result)
{
}

csaw::TemplateTypePtr csaw::TemplateType::Get(const TypePtr& base, const std::vector<TypePtr>& args)
{
    std::string name = base->Name + '<';
    for (size_t i = 0; i < args.size(); ++i)
    {
        if (i > 0) name += ", ";
        name += args[i]->Name;
    }
    name += '>';

    auto& type = Types[name];
    if (!type)
        type = std::make_shared<TemplateType>(name, base, args);
    return std::dynamic_pointer_cast<TemplateType>(type);
}

csaw::TemplateType::TemplateType(const std::string& name, const TypePtr& base, const std::vector<TypePtr>& args)
    : Type(name, INFO_IS_TEMP), Base(base), Args(args)
{
}
