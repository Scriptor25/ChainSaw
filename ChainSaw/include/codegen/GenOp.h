#pragma once

#include <codegen/Context.h>
#include <codegen/Value.h>

#include <lang/Def.h>

namespace csaw::codegen
{
	ValuePtr CodeGenAssign(ContextPtr context, ValuePtr assignee, ValuePtr value);

	ValuePtr CodeGenOpLT(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpGT(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpLE(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpGE(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpEQ(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpNE(ContextPtr context, ValuePtr left, ValuePtr right);

	ValuePtr CodeGenOpAdd(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpSub(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpMul(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpDiv(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpRem(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpAnd(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpOr(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpXOr(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpLAnd(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpLOr(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpShL(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpShR(ContextPtr context, ValuePtr left, ValuePtr right);

	ValuePtr CodeGenOpNeg(ContextPtr context, ValuePtr value);
	ValuePtr CodeGenOpNot(ContextPtr context, ValuePtr value);
	ValuePtr CodeGenOpInv(ContextPtr context, ValuePtr value);
}
