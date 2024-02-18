#include <codegen/Context.h>

csaw::codegen::BranchPtr csaw::codegen::Context::CreateBranch()
{
	auto function = GetCurrentFunction();
	return function->CreateBranch();
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

csaw::codegen::ValuePtr csaw::codegen::Context::CreateMerge(ValuePtr condition, TypePtr type, BranchPtr _true, BranchPtr _false)
{
	auto result = std::make_shared<Value>(type);
	auto inst = std::make_shared<MergeInst>(condition, _true, _false, result);
	m_InsertPoint->Insert(inst);
	return result;
}
