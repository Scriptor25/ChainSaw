#pragma once

#include <runtime/Runtime.h>

namespace csawstd
{
	using namespace csaw::runtime;

	void InitRandom();

	ValuePtr random(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
}
