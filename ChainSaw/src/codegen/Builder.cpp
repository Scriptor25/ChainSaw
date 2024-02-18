#include <codegen/Context.h>

csaw::codegen::ConstNumPtr csaw::codegen::Context::GetConstNum(double value)
{
	return std::make_shared<ConstNum>(GetNumType(), value);
}

csaw::codegen::ConstStrPtr csaw::codegen::Context::GetConstStr(const std::string& value)
{
	return std::make_shared<ConstStr>(GetStrType(), value);
}

void csaw::codegen::Context::SetInsertPoint(InstructionPtr insertpoint)
{
	m_InsertPoint = insertpoint;
}

void csaw::codegen::Context::SetInsertGlobal()
{
	m_InsertPoint = m_GlobalInsertPoint;
}

csaw::codegen::FunctionPtr csaw::codegen::Context::GetCurrentFunction()
{
	InstructionPtr ptr = m_InsertPoint;
	while (ptr->Prev)
		ptr = ptr->Prev;
	return std::dynamic_pointer_cast<Function>(ptr);
}

void csaw::codegen::Context::CreateVar(const std::string& name, TypePtr type, ValuePtr value)
{
	m_Variables[name] = type;
	auto inst = std::make_shared<CreateVarInst>(name, type, value);
	m_InsertPoint->Insert(inst);
}

void csaw::codegen::Context::CreateEmptyRet()
{
	auto inst = std::make_shared<RetInst>(ValuePtr());
	m_InsertPoint->Insert(inst);
}

void csaw::codegen::Context::CreateRet(ValuePtr value)
{
	auto inst = std::make_shared<RetInst>(value);
	m_InsertPoint->Insert(inst);
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateCall(FunctionPtr function, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	auto result = std::make_shared<Value>(function->Type->Result);
	auto inst = std::make_shared<CallInst>(function, callee, args, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateGetElement(ValuePtr thing, const std::string& element)
{
	auto result = std::make_shared<Value>(std::dynamic_pointer_cast<ThingType>(thing->Type)->Elements[element]);
	auto inst = std::make_shared<GetElementInst>(thing, element, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateGetVar(const std::string& name)
{
	auto result = std::make_shared<Value>(m_Variables[name]);
	auto inst = std::make_shared<GetVarInst>(name, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAssign(ValuePtr var, ValuePtr value)
{
	auto inst = std::make_shared<AssignVarInst>(var, value);
	m_InsertPoint->Insert(inst);
	return var;
}
