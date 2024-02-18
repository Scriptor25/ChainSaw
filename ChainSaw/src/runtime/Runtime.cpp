#include <runtime/Runtime.h>

using namespace csaw::codegen;

csaw::runtime::Runtime::Runtime(ContextPtr context)
	: Runtime(context, nullptr)
{
}

csaw::runtime::Runtime::Runtime(RuntimePtr runtime)
	: Runtime(runtime->m_Context, runtime)
{
}

csaw::runtime::Runtime::Runtime(ContextPtr context, RuntimePtr runtime)
	: m_Context(context), m_Parent(runtime)
{
}

ValuePtr csaw::runtime::Runtime::Call(const std::string& name, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	std::vector<csaw::codegen::TypePtr> argtypes;
	for (auto& arg : args)
		argtypes.push_back(arg->Type);

	auto function = m_Context->GetFunction(name, callee->Type, argtypes);
	return Call(function, callee, args);
}

csaw::runtime::RuntimePtr csaw::runtime::Runtime::GetGlobal()
{
	if (m_Parent)
		return m_Parent->GetGlobal();
	return shared_from_this();
}

ValuePtr csaw::runtime::Runtime::Create(const std::string& name, ValuePtr value)
{
	if (GetEntry(name)) throw;
	return m_Variables[name] = value;
}

ValuePtr csaw::runtime::Runtime::Get(const std::string& name)
{
	auto& entry = GetEntry(name);
	if (!entry) throw;
	return entry;
}

ValuePtr csaw::runtime::Runtime::Set(const std::string& name, ValuePtr value)
{
	auto& entry = GetEntry(name);
	if (!entry) throw;
	return entry = value;
}

ValuePtr& csaw::runtime::Runtime::GetEntry(const std::string& name)
{
	if (auto& entry = m_Variables[name])
		return entry;
	return m_Parent->GetEntry(name);
}

ValuePtr csaw::runtime::Runtime::Call(FunctionPtr function, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	function->Args;
	function->Branches;
	function->Callee;
	function->IsConstructor;
	function->Name;
	function->Next;
	function->Prev;

	Runtime runtime(GetGlobal());

	for (size_t i = 0; i < function->Args.size(); i++)
		runtime.Create(function->Args[i]->Name, args[i]);

	if ()
	if (callee)
		runtime.Create("my", callee);
}
