#include <csaw/Builder.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Type.hpp>

bool csaw::Signature::operator==(const Signature& other) const
{
    if (Name != other.Name
        || Parent != other.Parent
        || Result != other.Result
        || Args.size() != other.Args.size()
        || IsVarargs != other.IsVarargs)
        return false;

    for (size_t i = 0; i < Args.size(); ++i)
        if (Args[i] != other.Args[i])
            return false;

    return true;
}

std::string csaw::Signature::Mangle(const bool obfusecate) const
{
    static std::hash<std::string> hasher;

    if (IsC)
        return Name;

    std::string name = Name;

    if (Parent)
        name += "$" + Parent->Name;

    name += '$' + std::to_string(Args.size());

    for (const auto& arg : Args)
        name += '$' + arg->Name;

    if (IsVarargs)
        name += "$V";

    if (Result)
        name += '$' + Result->Name;

    if (obfusecate)
        return std::to_string(hasher(name));

    return name;
}

bool csaw::Signature::IsConstructor() const
{
    return !Parent && !Result;
}

csaw::FunctionTypePtr csaw::Signature::GetFunctionType() const
{
    return FunctionType::Get(Args, IsVarargs, Parent, Result);
}
