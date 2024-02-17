#pragma once

#include <runtime/Runtime.h>

namespace csawstd
{
	using namespace csaw::runtime;

	void InitList();

	struct List
	{
		static List* FromStr(const std::string& str);

		std::string ToStr();
		void Add(ValuePtr value);

		std::vector<ValuePtr> Values;
	};

	ValuePtr list(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr add(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr size(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr get(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
}
