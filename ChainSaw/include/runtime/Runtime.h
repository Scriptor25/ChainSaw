#pragma once

#include <codegen/Context.h>

#include <string>

namespace csaw::runtime
{
	using namespace csaw::codegen;

	typedef std::shared_ptr<class Runtime> RuntimePtr;

	class Runtime
		: std::enable_shared_from_this<Runtime>
	{
	public:
		Runtime(ContextPtr context);
		Runtime(RuntimePtr runtime);
		Runtime(ContextPtr context, RuntimePtr runtime);

		ValuePtr Call(const std::string& name, ValuePtr callee, const std::vector<ValuePtr>& args);

	private:

		RuntimePtr GetGlobal();

		ValuePtr Create(const std::string& name, ValuePtr value);
		ValuePtr Get(const std::string& name);
		ValuePtr Set(const std::string& name, ValuePtr value);
		ValuePtr& GetEntry(const std::string& name);

		ValuePtr Call(FunctionPtr function, ValuePtr callee, const std::vector<ValuePtr>& args);

	private:
		ContextPtr m_Context;
		RuntimePtr m_Parent;

		std::map<std::string, ValuePtr> m_Variables;
	};
}
