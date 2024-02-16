#include <runtime/Operators.h>
#include <runtime/Const.h>

bool csaw::runtime::EvalOpBool(EnvironmentPtr env, ValuePtr value)
{
	auto type = value->GetType();

	if (type == "num")
		return value->GetNum() != 0;

	if (type == "chr")
		return value->GetChr() != 0;

	if (type == "str")
		return !value->GetStr().empty();

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpLT(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() < right->GetNum());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpGT(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() > right->GetNum());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpLE(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() <= right->GetNum());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpGE(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() >= right->GetNum());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpEQ(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() == right->GetNum());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpNE(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() != right->GetNum());

	if (ltype == rtype && rtype == "str")
		return ConstNum::Ptr(left->GetStr() != right->GetStr());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpAdd(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() + right->GetNum());

	if (ltype == "str" && rtype == "num")
		return ConstStr::Ptr(left->GetStr() + std::to_string(right->GetNum()));

	if (ltype == "num" && rtype == "str")
		return ConstStr::Ptr(std::to_string(left->GetNum()) + right->GetStr());

	if (ltype == rtype && rtype == "str")
		return ConstStr::Ptr(left->GetStr() + right->GetStr());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpSub(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() - right->GetNum());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpMul(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() * right->GetNum());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpDiv(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() / right->GetNum());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpRem(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(std::fmod(left->GetNum(), right->GetNum()));

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpAnd(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(long(left->GetNum()) & long(right->GetNum()));

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpOr(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(long(left->GetNum()) | long(right->GetNum()));

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpXOr(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(long(left->GetNum()) ^ long(right->GetNum()));

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpLAnd(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() && right->GetNum());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpLOr(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(left->GetNum() || right->GetNum());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpShL(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(long(left->GetNum()) << long(right->GetNum()));

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpShR(EnvironmentPtr env, ValuePtr left, ValuePtr right)
{
	auto ltype = left->GetType();
	auto rtype = right->GetType();

	if (ltype == rtype && rtype == "num")
		return ConstNum::Ptr(long(left->GetNum()) >> long(right->GetNum()));

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpAddOne(EnvironmentPtr env, ValuePtr value)
{
	auto type = value->GetType();

	if (type == "num")
		return ConstNum::Ptr(value->GetNum() + 1.0);

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpSubOne(EnvironmentPtr env, ValuePtr value)
{
	auto type = value->GetType();

	if (type == "num")
		return ConstNum::Ptr(value->GetNum() - 1.0);

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpNeg(EnvironmentPtr env, ValuePtr value)
{
	auto type = value->GetType();

	if (type == "num")
		return ConstNum::Ptr(-value->GetNum());

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpLNot(EnvironmentPtr env, ValuePtr value)
{
	return ConstNum::Ptr(!EvalOpBool(env, value));
}

csaw::runtime::ValuePtr csaw::runtime::EvalOpNot(EnvironmentPtr env, ValuePtr value)
{
	auto type = value->GetType();

	if (type == "num")
		return ConstNum::Ptr(~long(value->GetNum()));

	throw;
}
