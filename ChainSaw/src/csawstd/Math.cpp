#include <csawstd/Math.h>
#include <runtime/Const.h>

using namespace csaw::runtime;

void csawstd::InitMath()
{
	Environment::CreateFunction(false, "floor", "num", { "num" }, false, "", floor);
	Environment::CreateFunction(false, "sqrt", "num", { "num" }, false, "", sqrt);
	Environment::CreateFunction(false, "infinity", "num", {}, false, "", infinity);
	Environment::CreateFunction(false, "tan", "num", { "num" }, false, "", tan);
	Environment::CreateFunction(false, "acos", "num", { "num" }, false, "", acos);
	Environment::CreateFunction(false, "atan2", "num", { "num", "num" }, false, "", atan2);
	Environment::CreateFunction(false, "abs", "num", { "num" }, false, "", abs);
	Environment::CreateFunction(false, "pow", "num", { "num", "num" }, false, "", pow);
}

ValuePtr csawstd::floor(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(std::floor(args[0]->GetNum()));
}

ValuePtr csawstd::sqrt(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(std::sqrt(args[0]->GetNum()));
}

ValuePtr csawstd::infinity(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(std::numeric_limits<double>().max());
}

ValuePtr csawstd::tan(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(std::tan(args[0]->GetNum()));
}

ValuePtr csawstd::acos(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(std::acos(args[0]->GetNum()));
}

ValuePtr csawstd::atan2(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(std::atan2(args[0]->GetNum(), args[1]->GetNum()));
}

ValuePtr csawstd::abs(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(std::abs(args[0]->GetNum()));
}

ValuePtr csawstd::pow(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(std::pow(args[0]->GetNum(), args[1]->GetNum()));
}
