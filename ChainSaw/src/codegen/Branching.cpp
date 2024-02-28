#include <codegen/Context.h>
#include <codegen/Instruction.h>

csaw::codegen::BranchPtr csaw::codegen::Context::CreateBranch(const std::string& name)
{
	auto function = GetInsertFunction();
	return function->CreateBranch(name);
}

void csaw::codegen::Context::CreateSplit(ValuePtr condition, BranchPtr _true, BranchPtr _false)
{
	auto inst = std::make_shared<SplitInst>(condition, _true, _false);
	m_InsertPoint->Insert(inst);
}

void csaw::codegen::Context::CreateFlow(BranchPtr branch)
{
	auto inst = std::make_shared<FlowInst>(branch);
	m_InsertPoint->Insert(inst);
}
