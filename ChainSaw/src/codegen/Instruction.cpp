#include <csaw/codegen/Instruction.hpp>

csaw::Instruction::~Instruction() = default;

void csaw::Instruction::Insert(const InstructionPtr &ptr)
{
    if (Next) Next->Insert(ptr);
    else Next = ptr;
}

csaw::InstSimple::InstSimple(const ValuePtr &result, const std::string &op, const std::vector<ValuePtr> &operands)
        : Result(result), Operator(op), Operands(operands)
{
}

csaw::InstCall::InstCall(const ValuePtr &result,
                         const FunctionPtr &function,
                         const ValuePtr &callee,
                         const std::vector<ValuePtr> &args)
        : Result(result), Function(function), Callee(callee), Args(args)
{
}

csaw::InstBranch::InstBranch(const ValuePtr &condition, const std::vector<BlockPtr> &blocks)
        : Condition(condition), Blocks(blocks)
{
}

csaw::InstRet::InstRet(const ValuePtr &value)
        : RetValue(value)
{
}
