#pragma once

#include <codegen/Instruction.h>
#include <codegen/Type.h>

#include <memory>
#include <string>
#include <vector>

namespace csaw::codegen
{
	struct Arg;
	struct Function;

	struct Branch;

	typedef std::shared_ptr<Arg> ArgPtr;
	typedef std::shared_ptr<Function> FunctionPtr;

	typedef std::shared_ptr<Branch> BranchPtr;

	struct Arg
	{
		Arg(const std::string& name, TypePtr type);

		std::string Name;
		TypePtr Type;
	};

	struct Function : Instruction
	{
		Function(const std::string& name, FunctionTypePtr type, bool isconstructor, const std::vector<ArgPtr>& args, TypePtr callee);

		BranchPtr CreateBranch();

		std::string Name;
		FunctionTypePtr Type;
		bool IsConstructor;
		std::vector<ArgPtr> Args;
		TypePtr Callee;

		std::vector<BranchPtr> Branches;
	};

	struct Branch : Instruction
	{
		Branch();
	};
}
