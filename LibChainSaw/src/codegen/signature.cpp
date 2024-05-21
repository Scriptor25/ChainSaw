#include <csaw/Builder.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Type.hpp>

std::string csaw::Signature::Mangle() const
{
    if (IsC) return Name;

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

    return name;
}

bool csaw::Signature::IsConstructor() const
{
    return !Parent && !Result;
}

csaw::FunctionTypePtr csaw::Signature::GetFunctionType() const
{
    return FunctionType::Get(Args, IsVarargs, Result);
}
