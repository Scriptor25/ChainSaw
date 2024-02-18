#include <codegen/Function.h>

csaw::codegen::Arg::Arg(const std::string& name, TypePtr type)
	: Name(name), Type(type)
{
}

csaw::codegen::Function::Function(const std::string& name, FunctionTypePtr type, bool isconstructor, const std::vector<ArgPtr>& args, TypePtr callee)
	: Name(name), Type(type), IsConstructor(isconstructor), Args(args), Callee(callee)
{
}

csaw::codegen::BranchPtr csaw::codegen::Function::CreateBranch()
{
	auto branch = std::make_shared<Branch>();
	branch->Prev = shared_from_this();
	Branches.push_back(branch);
	return branch;
}

csaw::codegen::Branch::Branch()
{
}
