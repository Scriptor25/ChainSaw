#include <lang/Parser.h>
#include <runtime/Const.h>

#include <csawstd/CSawStd.h>

#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
	csawstd::InitLib();

	std::string filename = "C:/Users/Felix Schreiber/Documents/Projects/C++/CSaw/csaw/mandel/main.csaw";
	std::ifstream stream(filename);
	if (!stream.is_open())
		std::cerr << "Failed to open stream from '" << filename << "'" << std::endl;
	auto env = csaw::runtime::Environment::Ptr(filename);
	csaw::lang::Parser::Parse(stream, env);
	stream.close();

	auto result = env->GetAndInvoke(env, "main", nullptr, {});
	std::cout << "Exit Code " << result << std::endl;

	return 0;
}
