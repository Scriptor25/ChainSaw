#pragma once

#include <codegen/Def.h>
#include <codegen/Instruction.h>

namespace csaw::codegen
{
	struct NativeInst : Instruction
	{
		NativeInst(NativeFunction function);

		std::ostream& Print(std::ostream& out) const override;

		NativeFunction Function;
	};
}
