#include <csawstd/Types.h>
#include <runtime/Const.h>

using namespace csaw::runtime;

void csawstd::InitTypes()
{
	Environment::CreateFunction(false, "num", "num", { "str" }, false, "", ston);
	Environment::CreateFunction(false, "num", "num", { "chr" }, false, "", cton);
	Environment::CreateFunction(false, "str", "str", { "num" }, false, "", ntos);
	Environment::CreateFunction(false, "str", "str", { "chr" }, false, "", ctos);
	Environment::CreateFunction(false, "chr", "chr", { "num" }, false, "", ntoc);
}

ValuePtr csawstd::ston(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(std::stod(args[0]->GetStr()));
}

ValuePtr csawstd::cton(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstNum::Ptr(args[0]->GetChr());
}

ValuePtr csawstd::ntos(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstStr::Ptr(std::to_string(args[0]->GetNum()));
}

ValuePtr csawstd::ntoc(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstChr::Ptr(char(args[0]->GetNum()));
}

ValuePtr csawstd::ctos(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	return ConstStr::Ptr(std::string(1, args[0]->GetChr()));
}
