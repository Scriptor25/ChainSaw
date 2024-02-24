#include <codegen/Instruction.h>
#include <codegen/Native.h>

void csaw::codegen::Instruction::Insert(InstructionPtr instruction)
{
	if (!Next)
	{
		Next = instruction;
		Next->Prev = shared_from_this();
	}
	else Next->Insert(instruction);
}

bool csaw::codegen::Instruction::IsTerminator() const
{
	return false;
}

csaw::codegen::CreateVarInst::CreateVarInst(const std::string& name, ValuePtr var, ValuePtr value)
	: Name(name), Var(var), Value(value)
{
}

csaw::codegen::RetInst::RetInst(ValuePtr value)
	: Value(value)
{
}

bool csaw::codegen::RetInst::IsTerminator() const
{
	return true;
}

csaw::codegen::SplitInst::SplitInst(ValuePtr condition, BranchPtr _true, BranchPtr _false)
	: Condition(condition), True(_true), False(_false)
{
}

bool csaw::codegen::SplitInst::IsTerminator() const
{
	return true;
}

csaw::codegen::FlowInst::FlowInst(BranchPtr branch)
	: Branch(branch)
{
}

bool csaw::codegen::FlowInst::IsTerminator() const
{
	return true;
}

csaw::codegen::CallInst::CallInst(FunctionPtr function, ValuePtr callee, const std::vector<ValuePtr>& args, ValuePtr result)
	: Function(function), Callee(callee), Args(args), Result(result)
{
}

csaw::codegen::GetElementInst::GetElementInst(ValuePtr thing, const std::string& element, ValuePtr result)
	: Thing(thing), Element(element), Result(result)
{
}

csaw::codegen::AssignVarInst::AssignVarInst(ValuePtr var, ValuePtr value)
	: Var(var), Value(value)
{
}

csaw::codegen::AddInst::AddInst(MathInstMode mode, ValuePtr left, ValuePtr right, ValuePtr result)
	: Mode(mode), Left(left), Right(right), Result(result)
{
}

csaw::codegen::SubInst::SubInst(MathInstMode mode, ValuePtr left, ValuePtr right, ValuePtr result)
	: Mode(mode), Left(left), Right(right), Result(result)
{
}

csaw::codegen::MulInst::MulInst(ValuePtr left, ValuePtr right, ValuePtr result)
	: Left(left), Right(right), Result(result)
{
}

csaw::codegen::DivInst::DivInst(ValuePtr left, ValuePtr right, ValuePtr result)
	: Left(left), Right(right), Result(result)
{
}

csaw::codegen::NegInst::NegInst(ValuePtr value, ValuePtr result)
	: Value(value), Result(result)
{
}

csaw::codegen::CmpInst::CmpInst(MathInstMode mode, CmpInstMode cmpmode, ValuePtr left, ValuePtr right, ValuePtr result)
	: Mode(mode), CMPMode(cmpmode), Left(left), Right(right), Result(result)
{
}

csaw::codegen::SelInst::SelInst(ValuePtr condition, ValuePtr _true, ValuePtr _false, ValuePtr result)
	: Condition(condition), True(_true), False(_false), Result(result)
{
}

csaw::codegen::LAndInst::LAndInst(ValuePtr left, ValuePtr right, ValuePtr result)
	: Left(left), Right(right), Result(result)
{
}

csaw::codegen::NativeInst::NativeInst(NativeFunction function)
	: Function(function)
{
}
