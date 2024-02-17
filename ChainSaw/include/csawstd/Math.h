#pragma once

#include <runtime/Runtime.h>

namespace csawstd
{
	using namespace csaw::runtime;

	void InitMath();

	ValuePtr floor(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr sqrt(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr infinity(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr tan(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr acos(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr atan2(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr abs(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr pow(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
}
