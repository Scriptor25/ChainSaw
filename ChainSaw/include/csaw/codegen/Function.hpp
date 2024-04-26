#pragma once

#include <csaw/codegen/Def.hpp>
#include <csaw/Type.hpp>

namespace csaw
{
    struct Function
    {
        Function(const std::string &name, const FunTypePtr &type, bool constructor);

        bool Implemented() const;

        void Insert(const BlockPtr &ptr);

        void SetEntry(const BlockPtr &ptr);

        std::string Name;
        FunTypePtr Type;
        bool Constructor;

        ValuePtr Me;
        std::vector<ArgPtr> Args;

        BlockPtr Entry;
        std::vector<BlockPtr> Blocks;
    };
}
