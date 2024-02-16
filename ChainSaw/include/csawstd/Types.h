#pragma once

#include <runtime/Runtime.h>

namespace csawstd
{
	using namespace csaw::runtime;

	void InitTypes();

	ValuePtr ston(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr cton(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr ntos(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr ntoc(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr ctos(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
}
