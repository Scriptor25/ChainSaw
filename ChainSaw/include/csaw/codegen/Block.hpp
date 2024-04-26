#pragma  once

#include <csaw/codegen/Def.hpp>

namespace csaw
{
    struct Block
    {
        static BlockPtr Create(const FunctionPtr &parent);

        explicit Block(const FunctionPtr &parent);

        void Insert(const InstructionPtr &ptr);

        FunctionPtr Parent;
        InstructionPtr Next;
    };
}
