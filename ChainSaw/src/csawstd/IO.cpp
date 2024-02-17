#include <csawstd/IO.h>
#include <runtime/Const.h>

using namespace csaw::runtime;

void csawstd::InitIO()
{
	Environment::CreateFunction(false, "printf", "", { "str" }, true, "", printf);
	Environment::CreateFunction(false, "readf", "str", { "str" }, true, "", readf);
}

ValuePtr csawstd::printf(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	std::string fmt = args[0]->GetStr();
	std::vector<std::string> format;
	size_t pos = 0;
	while ((pos = fmt.find("{}")) != std::string::npos)
	{
		auto segment = fmt.substr(0, pos);
		fmt.erase(0, pos + 2);
		format.push_back(segment);
	}
	format.push_back(fmt);

	for (size_t i = 0; i < format.size(); i++)
	{
		if (i > 0)
			std::cout << args[i];
		std::cout << format[i];
	}

	return ValuePtr();
}

ValuePtr csawstd::readf(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	printf(env, callee, args);

	std::string line;
	std::getline(std::cin, line);

	return ConstStr::Ptr(line);
}