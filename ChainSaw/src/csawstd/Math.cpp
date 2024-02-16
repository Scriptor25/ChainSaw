#include <csawstd/Math.h>
#include <runtime/Const.h>

using namespace csaw::runtime;

void csawstd::InitMath()
{
	Environment::CreateFunction(false, "floor", "num", { "num" }, false, "", csawstd::floor);
	Environment::CreateFunction(false, "sqrt", "num", { "num" }, false, "", csawstd::sqrt);
}

ValuePtr csawstd::floor(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(std::floor(args[0]->GetNum()));
}

ValuePtr csawstd::sqrt(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(std::sqrt(args[0]->GetNum()));
}
