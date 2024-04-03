#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

#include <iostream>

csaw::codegen::Function::Function(const std::string& name, FunctionTypePtr type, bool isconstructor, const std::vector<ArgPtr>& args, TypePtr callee)
	: Name(name), Type(type), IsConstructor(isconstructor), Args(args), Callee(callee)
{
}

std::ostream& csaw::codegen::Function::Print(std::ostream& out) const
{
	Type->Print(out) << " " << Name << " = (";

	bool first = true;
	for (auto& arg : Args)
	{
		if (first) first = false;
		else out << ", ";
		arg->Print(out);
	}
	out << ")";

	if (IsConstructor) out << " $";
	if (Callee && !Callee->IsEmpty()) out << " -> " << Callee->GetName();
	if (!Next) return out << ";";

	out << std::endl << "{" << std::endl;

	InstructionPtr ptr = Next;
	while (ptr)
	{
		ptr->Print(out << "\t") << std::endl;
		ptr = ptr->Next;
	}

	for (auto& branch : Branches)
		branch->Print(out) << std::endl;

	return out << "}";
}

std::ostream& csaw::codegen::Function::PrintShort(std::ostream& out) const
{
	return Type->Print(out) << " " << Name;
}

csaw::codegen::BranchPtr csaw::codegen::Function::CreateBranch(const std::string& name)
{
	static size_t i = 0;

	std::string n = name;
	if (n.empty())
		n = std::to_string(i++);

	auto branch = std::make_shared<Branch>(n);
	branch->Prev = shared_from_this();
	Branches.push_back(branch);
	return branch;
}

csaw::codegen::Branch::Branch(const std::string& name)
	: Name(name)
{
}

bool csaw::codegen::Branch::HasTerminator() const
{
	InstructionPtr ptr = Next;
	while (ptr)
	{
		if (ptr->IsTerminator())
			return true;
		ptr = ptr->Next;
	}

	return false;
}

std::ostream& csaw::codegen::Branch::Print(std::ostream& out) const
{
	out << "$" << Name << ":" << std::endl;

	InstructionPtr ptr = Next;
	while (ptr)
	{
		ptr->Print(out << "\t") << std::endl;
		ptr = ptr->Next;
	}

	return out;
}
