#include <codegen/Context.h>
#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

csaw::codegen::ValuePtr csaw::codegen::Context::CreateSubNN(ValuePtr left, ValuePtr right)
{
	auto result = std::make_shared<Value>(GetNumType());
	auto inst = std::make_shared<SubInst>(MODE_NN, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateSubCC(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateSubCN(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateSubNC(ValuePtr left, ValuePtr right)
{
	throw;
}
