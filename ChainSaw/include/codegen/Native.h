#pragma once

#include <codegen/Instruction.h>
#include <codegen/Value.h>

#include <runtime/Runtime.h>

#include <functional>
#include <memory>

namespace csaw::runtime
{
	typedef std::shared_ptr<class Runtime> RuntimePtr;
}

namespace csaw::codegen
{
	typedef std::function<ConstPtr(csaw::runtime::RuntimePtr runtime, ConstPtr callee, const std::vector<ConstPtr>& args)> NativeFunction;

	struct NativeInst : Instruction
	{
		NativeInst(NativeFunction function);

		std::ostream& Print(std::ostream& out) const override;

		NativeFunction Function;
	};

	typedef std::shared_ptr<NativeInst> NativeInstPtr;
}
