#include <codegen/Context.h>

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateAddNN(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<AddInst>(MODE_NN, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateAddCC(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateAddSS(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetStrType());
	auto inst = std::make_shared<AddInst>(MODE_SS, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateAddNS(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetStrType());
	auto inst = std::make_shared<AddInst>(MODE_NS, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateAddCS(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateAddSN(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetStrType());
	auto inst = std::make_shared<AddInst>(MODE_SN, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateAddSC(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateAddCN(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateAddNC(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}
