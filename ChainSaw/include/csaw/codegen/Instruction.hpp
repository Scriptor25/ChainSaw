#pragma once

#include <csaw/codegen/Def.hpp>

#include <vector>

namespace csaw
{
    struct Instruction
    {
        virtual ~Instruction();

        void Insert(const InstructionPtr &ptr);

        InstructionPtr Next;
    };

    struct InstSimple : Instruction
    {
        InstSimple(const ValuePtr &result, const std::string &op, const std::vector<ValuePtr> &operands);

        ValuePtr Result;
        std::string Operator;
        std::vector<ValuePtr> Operands;
    };

    struct InstCall : Instruction
    {
        InstCall(const ValuePtr &result,
                 const FunctionPtr &function,
                 const ValuePtr &callee,
                 const std::vector<ValuePtr> &args);

        ValuePtr Result;
        FunctionPtr Function;
        ValuePtr Callee;
        std::vector<ValuePtr> Args;
    };

    struct InstBranch : Instruction
    {
        InstBranch(const ValuePtr &condition, const std::vector<BlockPtr> &blocks);

        ValuePtr Condition;
        std::vector<BlockPtr> Blocks;
    };

    struct InstRet : Instruction
    {
        InstRet(const ValuePtr &value);

        ValuePtr RetValue;
    };
}
