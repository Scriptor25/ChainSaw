#pragma once

#include <runtime/Runtime.h>

namespace csawstd
{
	using namespace csaw::runtime;

	void InitMath();

	ValuePtr floor(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr sqrt(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
}
