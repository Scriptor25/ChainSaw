#include <codegen/Instruction.h>
#include <codegen/Native.h>

#include <iostream>

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

std::ostream& csaw::codegen::Instruction::Print() const
{
	return Print(std::cout);
}

csaw::codegen::CreateVarInst::CreateVarInst(const std::string& name, ValuePtr var, ValuePtr value)
	: Name(name), Var(var), Value(value)
{
}

std::ostream& csaw::codegen::CreateVarInst::Print(std::ostream& out) const
{
	if (Name.empty()) return out << "placeholder";

	// var = create name, value
	return Value->Print(Var->Print(out) << " = create " << Name << ", ");
}

csaw::codegen::RetInst::RetInst(ValuePtr value)
	: Value(value)
{
}

bool csaw::codegen::RetInst::IsTerminator() const
{
	return true;
}

std::ostream& csaw::codegen::RetInst::Print(std::ostream& out) const
{
	// ret value
	out << "ret";
	if (Value) Value->Print(out << " ");
	return out;
}

csaw::codegen::SplitInst::SplitInst(ValuePtr condition, BranchPtr _true, BranchPtr _false)
	: Condition(condition), True(_true), False(_false)
{
}

bool csaw::codegen::SplitInst::IsTerminator() const
{
	return true;
}

std::ostream& csaw::codegen::SplitInst::Print(std::ostream& out) const
{
	// split condition, true, false
	return Condition->Print(out << "split ") << ", $" << True->Name << ", $" << False->Name;
}

csaw::codegen::FlowInst::FlowInst(BranchPtr branch)
	: Branch(branch)
{
}

bool csaw::codegen::FlowInst::IsTerminator() const
{
	return true;
}

std::ostream& csaw::codegen::FlowInst::Print(std::ostream& out) const
{
	// flow branch
	return out << "flow $" << Branch->Name;
}

csaw::codegen::CallInst::CallInst(FunctionPtr function, ValuePtr callee, const std::vector<ValuePtr>& args, ValuePtr result)
	: Function(function), Callee(callee), Args(args), Result(result)
{
}

std::ostream& csaw::codegen::CallInst::Print(std::ostream& out) const
{
	// result = call function, callee, args

	if (!Result->Type->IsEmpty()) Result->Print(out) << " = ";

	Function->PrintShort(out << "call ") << ", ";
	if (Callee) Callee->Print(out);
	else out << "empty";

	for (auto& arg : Args)
		arg->Print(out << ", ");

	return out;
}

csaw::codegen::GetElementInst::GetElementInst(ValuePtr thing, const std::string& element, ValuePtr result)
	: Thing(thing), Element(element), Result(result)
{
}

std::ostream& csaw::codegen::GetElementInst::Print(std::ostream& out) const
{
	// result = getelement thing, element
	return Thing->Print(Result->Print(out) << " = getelement ") << ", " << Element;
}

csaw::codegen::AssignVarInst::AssignVarInst(ValuePtr var, ValuePtr value)
	: Var(var), Value(value)
{
}

std::ostream& csaw::codegen::AssignVarInst::Print(std::ostream& out) const
{
	// assign var, value
	return Value->Print(Var->Print(out << "assign ") << ", ");
}

csaw::codegen::AddInst::AddInst(MathInstMode mode, ValuePtr left, ValuePtr right, ValuePtr result)
	: Mode(mode), Left(left), Right(right), Result(result)
{
}

std::ostream& csaw::codegen::AddInst::Print(std::ostream& out) const
{
	// result = add mode, left, right;
	return Right->Print(Left->Print(Result->Print(out) << " = add " << Mode << ", ") << ", ");
}

csaw::codegen::SubInst::SubInst(MathInstMode mode, ValuePtr left, ValuePtr right, ValuePtr result)
	: Mode(mode), Left(left), Right(right), Result(result)
{
}

std::ostream& csaw::codegen::SubInst::Print(std::ostream& out) const
{
	// result = sub mode, left, right;
	return Right->Print(Left->Print(Result->Print(out) << " = sub " << Mode << ", ") << ", ");
}

csaw::codegen::MulInst::MulInst(ValuePtr left, ValuePtr right, ValuePtr result)
	: Left(left), Right(right), Result(result)
{
}

std::ostream& csaw::codegen::MulInst::Print(std::ostream& out) const
{
	// result = mul left, right;
	return Right->Print(Left->Print(Result->Print(out) << " = mul ") << ", ");
}

csaw::codegen::DivInst::DivInst(ValuePtr left, ValuePtr right, ValuePtr result)
	: Left(left), Right(right), Result(result)
{
}

std::ostream& csaw::codegen::DivInst::Print(std::ostream& out) const
{
	// result = div left, right;
	return Right->Print(Left->Print(Result->Print(out) << " = div ") << ", ");
}

csaw::codegen::NegInst::NegInst(ValuePtr value, ValuePtr result)
	: Value(value), Result(result)
{
}

std::ostream& csaw::codegen::NegInst::Print(std::ostream& out) const
{
	// result = neg value
	return Value->Print(Result->Print(out) << " = ");
}

csaw::codegen::CmpInst::CmpInst(MathInstMode mode, CmpInstMode cmpmode, ValuePtr left, ValuePtr right, ValuePtr result)
	: Mode(mode), CMPMode(cmpmode), Left(left), Right(right), Result(result)
{
}

std::ostream& csaw::codegen::CmpInst::Print(std::ostream& out) const
{
	// result = cmp cmpmode, mode, left, right
	return Right->Print(Left->Print(Result->Print(out) << " = cmp " << CMPMode << ", " << Mode << ", ") << ", ");
}

csaw::codegen::SelInst::SelInst(ValuePtr condition, ValuePtr _true, ValuePtr _false, ValuePtr result)
	: Condition(condition), True(_true), False(_false), Result(result)
{
}

std::ostream& csaw::codegen::SelInst::Print(std::ostream& out) const
{
	// result = sel condition, true, false
	return False->Print(True->Print(Condition->Print(Result->Print(out) << " = sel ") << ", ") << ", ");
}

csaw::codegen::LAndInst::LAndInst(ValuePtr left, ValuePtr right, ValuePtr result)
	: Left(left), Right(right), Result(result)
{
}

std::ostream& csaw::codegen::LAndInst::Print(std::ostream& out) const
{
	// result = land left, right
	return Right->Print(Left->Print(Result->Print(out) << " = land ") << ", ");
}

csaw::codegen::ShLInst::ShLInst(ValuePtr left, ValuePtr right, ValuePtr result)
	: Left(left), Right(right), Result(result)
{
}

std::ostream& csaw::codegen::ShLInst::Print(std::ostream& out) const
{
	// result = shl left, right
	return Right->Print(Left->Print(Result->Print(out) << " = shl ") << ", ");
}

csaw::codegen::AndInst::AndInst(ValuePtr left, ValuePtr right, ValuePtr result)
	: Left(left), Right(right), Result(result)
{
}

std::ostream& csaw::codegen::AndInst::Print(std::ostream& out) const
{
	// result = and left, right
	return Right->Print(Left->Print(Result->Print(out) << " = and ") << ", ");
}

csaw::codegen::OrInst::OrInst(ValuePtr left, ValuePtr right, ValuePtr result)
	: Left(left), Right(right), Result(result)
{
}

std::ostream& csaw::codegen::OrInst::Print(std::ostream& out) const
{
	// result = and left, right
	return Right->Print(Left->Print(Result->Print(out) << " = or ") << ", ");
}

csaw::codegen::NativeInst::NativeInst(NativeFunction function)
	: Function(function)
{
}

std::ostream& csaw::codegen::NativeInst::Print(std::ostream& out) const
{
	throw;
}
