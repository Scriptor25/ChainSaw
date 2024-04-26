#include <csaw/codegen/Block.hpp>
#include <csaw/codegen/Function.hpp>
#include <csaw/codegen/Instruction.hpp>

csaw::BlockPtr csaw::Block::Create(const FunctionPtr &parent)
{
    const auto block = std::make_shared<Block>(parent);
    parent->Insert(block);
    return block;
}

csaw::Block::Block(const FunctionPtr &parent)
        : Parent(parent)
{
}

void csaw::Block::Insert(const InstructionPtr &ptr)
{
    if (Next) Next->Insert(ptr);
    else Next = ptr;
}
