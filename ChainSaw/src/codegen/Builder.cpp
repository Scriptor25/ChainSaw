#include <codegen/Context.h>
#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

csaw::codegen::ConstNumPtr csaw::codegen::Context::GetConstNum(double value)
{
	return std::make_shared<ConstNum>(GetNumType(), value);
}

csaw::codegen::ConstChrPtr csaw::codegen::Context::GetConstChr(char value)
{
	return std::make_shared<ConstChr>(GetChrType(), value);
}

csaw::codegen::ConstStrPtr csaw::codegen::Context::GetConstStr(const std::string& value)
{
	return std::make_shared<ConstStr>(GetStrType(), value);
}

csaw::codegen::ConstThingPtr csaw::codegen::Context::GetConstThing(const std::string& type, const std::map<std::string, ValuePtr>& elements)
{
	return std::make_shared<ConstThing>(GetThingType(type), elements);
}

void csaw::codegen::Context::SetInsertPoint(InstructionPtr insertpoint)
{
	m_InsertPoint = insertpoint;
}

void csaw::codegen::Context::SetInsertGlobal()
{
	m_InsertPoint = m_GlobalInsertPoint;
}

bool csaw::codegen::Context::IsInsertGlobal() const
{
	return m_InsertPoint == m_GlobalInsertPoint;
}

csaw::codegen::InstructionPtr csaw::codegen::Context::GetInsertPoint()
{
	return m_InsertPoint;
}

csaw::codegen::BranchPtr csaw::codegen::Context::GetInsertBranch() const
{
	return std::dynamic_pointer_cast<Branch>(m_InsertPoint);
}

csaw::codegen::FunctionPtr csaw::codegen::Context::GetInsertFunction()
{
	InstructionPtr ptr = m_InsertPoint;
	while (ptr->Prev)
		ptr = ptr->Prev;
	return std::dynamic_pointer_cast<Function>(ptr);
}

void csaw::codegen::Context::CreateVar(const std::string& name, TypePtr type, ValuePtr value)
{
	if (!type) throw;
	if (!value) value = Value::Default(type, name);
	CreateVariable(name, value);
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
	auto& type = function->Type->Result;
	auto result = type->IsEmpty() ? ValuePtr() : Value::Default(type);
	auto inst = std::make_shared<CallInst>(function, callee, args, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateGetElement(ValuePtr thing, const std::string& element)
{
	auto result = Value::Default(thing->Type->AsThing()->Elements[element]);
	auto inst = std::make_shared<GetElementInst>(thing, element, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAssign(ValuePtr var, ValuePtr value)
{
	auto inst = std::make_shared<AssignVarInst>(var, value);
	m_InsertPoint->Insert(inst);
	return var;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateSel(ValuePtr condition, TypePtr type, ValuePtr _true, ValuePtr _false)
{
	auto result = Value::Default(type);
	auto inst = std::make_shared<SelInst>(condition, _true, _false, result);
	m_InsertPoint->Insert(inst);
	return result;
}
