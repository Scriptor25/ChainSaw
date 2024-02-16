#pragma once

#include <runtime/Runtime.h>

namespace csaw::runtime
{
	bool EvalOpBool(EnvironmentPtr env, ValuePtr value);

	ValuePtr EvalOpLT(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpGT(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpLE(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpGE(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpEQ(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpNE(EnvironmentPtr env, ValuePtr left, ValuePtr right);

	ValuePtr EvalOpAdd(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpSub(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpMul(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpDiv(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpRem(EnvironmentPtr env, ValuePtr left, ValuePtr right);

	ValuePtr EvalOpAnd(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpOr(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpXOr(EnvironmentPtr env, ValuePtr left, ValuePtr right);

	ValuePtr EvalOpLAnd(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpLOr(EnvironmentPtr env, ValuePtr left, ValuePtr right);

	ValuePtr EvalOpShL(EnvironmentPtr env, ValuePtr left, ValuePtr right);
	ValuePtr EvalOpShR(EnvironmentPtr env, ValuePtr left, ValuePtr right);

	ValuePtr EvalOpAddOne(EnvironmentPtr env, ValuePtr value);
	ValuePtr EvalOpSubOne(EnvironmentPtr env, ValuePtr value);

	ValuePtr EvalOpNeg(EnvironmentPtr env, ValuePtr value);
	ValuePtr EvalOpLNot(EnvironmentPtr env, ValuePtr value);
	ValuePtr EvalOpNot(EnvironmentPtr env, ValuePtr value);
}
