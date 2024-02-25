#pragma once

#include <codegen/Def.h>
#include <lang/Def.h>

namespace csaw::codegen
{
	ValueRefPtr CodeGenAssign(ContextPtr context, ValueRefPtr assignee, ValueRefPtr value);

	ValueRefPtr CodeGenOpLT(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpGT(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpLE(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpGE(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpEQ(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpNE(ContextPtr context, ValueRefPtr left, ValueRefPtr right);

	ValueRefPtr CodeGenOpAdd(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpSub(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpMul(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpDiv(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpRem(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpAnd(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpOr(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpXOr(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpLAnd(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpLOr(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpShL(ContextPtr context, ValueRefPtr left, ValueRefPtr right);
	ValueRefPtr CodeGenOpShR(ContextPtr context, ValueRefPtr left, ValueRefPtr right);

	ValueRefPtr CodeGenOpNeg(ContextPtr context, ValueRefPtr value);
	ValueRefPtr CodeGenOpNot(ContextPtr context, ValueRefPtr value);
	ValueRefPtr CodeGenOpInv(ContextPtr context, ValueRefPtr value);
}
