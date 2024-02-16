#pragma once

#include <runtime/Runtime.h>

namespace csawstd
{
	using namespace csaw::runtime;

	void InitIO();

	ValuePtr printf(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr readf(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
}
