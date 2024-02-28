#include <codegen/Context.h>
#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAddNN(ValuePtr left, ValuePtr right)
{
	auto result = std::make_shared<Value>(GetNumType());
	auto inst = std::make_shared<AddInst>(MODE_NN, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAddCC(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAddSS(ValuePtr left, ValuePtr right)
{
	auto result = std::make_shared<Value>(GetStrType());
	auto inst = std::make_shared<AddInst>(MODE_SS, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAddNS(ValuePtr left, ValuePtr right)
{
	auto result = std::make_shared<Value>(GetStrType());
	auto inst = std::make_shared<AddInst>(MODE_NS, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAddCS(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAddSN(ValuePtr left, ValuePtr right)
{
	auto result = std::make_shared<Value>(GetStrType());
	auto inst = std::make_shared<AddInst>(MODE_SN, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAddSC(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAddCN(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAddNC(ValuePtr left, ValuePtr right)
{
	throw;
}
