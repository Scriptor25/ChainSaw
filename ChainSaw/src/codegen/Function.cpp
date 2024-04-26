#include <csaw/codegen/Value.hpp>
#include <csaw/codegen/Function.hpp>

#include <algorithm>

csaw::Function::Function(const std::string &name, const FunTypePtr &type, const bool constructor)
        : Name(name), Type(type), Constructor(constructor)
{
    if (type->Callee || Constructor)
        Me = std::make_shared<Value>("me", type->Callee);

    Args.reserve(type->Args.size());
    for (size_t i = 0; i < type->Args.size(); ++i)
        Args.push_back(std::make_shared<Arg>(std::to_string(i), type->Args[i]));
}

bool csaw::Function::Implemented() const
{
    return !Blocks.empty();
}

void csaw::Function::Insert(const BlockPtr &ptr)
{
    Blocks.push_back(ptr);
}

void csaw::Function::SetEntry(const csaw::BlockPtr &ptr)
{
    Entry = ptr;
    if (std::find(Blocks.begin(), Blocks.end(), ptr) == Blocks.end())
        Blocks.push_back(ptr);
}
