#pragma once

#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

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

	struct Arg : Value
	{
		Arg(TypePtr type, const std::string& name = "");
	};

	struct Function : Instruction
	{
		Function(const std::string& name, FunctionTypePtr type, bool isconstructor, const std::vector<ArgPtr>& args, TypePtr callee);

		std::ostream& Print(std::ostream& out) const override;
		std::ostream& PrintShort(std::ostream& out) const;

		BranchPtr CreateBranch(const std::string& name);

		std::string Name;
		FunctionTypePtr Type;
		bool IsConstructor;
		std::vector<ArgPtr> Args;
		TypePtr Callee;

		std::vector<BranchPtr> Branches;
	};

	struct Branch : Instruction
	{
		Branch(const std::string& name);
		bool HasTerminator() const;

		std::ostream& Print(std::ostream& out) const override;

		std::string Name;
	};
}
