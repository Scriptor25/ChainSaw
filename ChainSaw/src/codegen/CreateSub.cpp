#include <codegen/Context.h>
#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateSubNN(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<SubInst>(MODE_NN, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateSubCC(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateSubCN(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateSubNC(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}
