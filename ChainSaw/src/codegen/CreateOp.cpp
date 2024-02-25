#include <codegen/Context.h>
#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateCmpLT_NN(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<CmpInst>(MODE_NN, CMP_LT, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateCmpGT_NN(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<CmpInst>(MODE_NN, CMP_GT, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateCmpLE_NN(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<CmpInst>(MODE_NN, CMP_LE, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateCmpEQ_NN(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<CmpInst>(MODE_NN, CMP_EQ, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateCmpNE_SS(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<CmpInst>(MODE_SS, CMP_NE, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateMul(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<MulInst>(left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateDiv(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<DivInst>(left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateRem(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateAnd(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<AndInst>(left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateOr(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<OrInst>(left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateXOr(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateLAnd(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<LAndInst>(left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateLOr(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateShL(ValueRefPtr left, ValueRefPtr right)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<ShLInst>(left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateShR(ValueRefPtr left, ValueRefPtr right)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateNeg(ValueRefPtr value)
{
	auto result = std::make_shared<ValueRef>(GetNumType());
	auto inst = std::make_shared<NegInst>(value, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateNot(ValueRefPtr value)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::Context::CreateInv(ValueRefPtr value)
{
	throw;
}
