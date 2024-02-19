//#define PRINT_PARSE

#include <lang/Parser.h>

#include <codegen/Context.h>
#include <codegen/GenStmt.h>

#include <runtime/Runtime.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

csaw::codegen::ContextPtr ctx;

static void callback(csaw::lang::StmtPtr stmt)
{
#ifdef PRINT_PARSE
	std::cout << stmt << std::endl;
#endif
	csaw::codegen::CodeGen(ctx, stmt);
}

using namespace csaw::lang;
using namespace csaw::codegen;
using namespace csaw::runtime;

namespace csaw::stdlib
{
	static ConstPtr floor(ContextPtr context, ConstPtr callee, const std::vector<ConstPtr>& args)
	{
		auto x = args[0]->AsNum();
		return std::make_shared<ConstNum>(x->Type->AsNum(), std::floor(x->Value));
	}

	static ConstPtr num(ContextPtr context, ConstPtr callee, const std::vector<ConstPtr>& args)
	{
		auto x = args[0]->AsStr();
		return std::make_shared<ConstNum>(context->GetNumType(), std::stod(x->Value));
	}

	static ConstPtr printf(ContextPtr context, ConstPtr callee, const std::vector<ConstPtr>& args)
	{
		std::string fmt = args[0]->AsStr()->Value;
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

		return ConstPtr();
	}

	std::mt19937_64 RNG;
	std::uniform_real_distribution<> UDIST;

	static void initrand()
	{
		RNG.seed(
			std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now()
				.time_since_epoch())
			.count());
	}

	static ConstPtr random(ContextPtr context, ConstPtr callee, const std::vector<ConstPtr>& args)
	{
		double r = UDIST(RNG);
		return std::make_shared<ConstNum>(context->GetNumType(), r);
	}

	static ConstPtr readf(ContextPtr context, ConstPtr callee, const std::vector<ConstPtr>& args)
	{
		printf(context, callee, args);

		std::string line;
		std::getline(std::cin, line);

		return std::make_shared<ConstStr>(context->GetStrType(), line);
	}
}

static void registerNative(RuntimePtr runtime)
{
	csaw::stdlib::initrand();

	runtime->RegisterNativeFunc(
		"floor",
		ctx->GetFunctionType(ctx->GetNumType(), { ctx->GetNumType() }, false),
		std::make_shared<NativeInst>(csaw::stdlib::floor));
	runtime->RegisterNativeFunc(
		"num",
		ctx->GetFunctionType(ctx->GetNumType(), { ctx->GetStrType() }, false),
		std::make_shared<NativeInst>(csaw::stdlib::num));
	runtime->RegisterNativeFunc(
		"printf",
		ctx->GetFunctionType(ctx->GetEmptyType(), { ctx->GetStrType() }, true),
		std::make_shared<NativeInst>(csaw::stdlib::printf));
	runtime->RegisterNativeFunc(
		"random",
		ctx->GetFunctionType(ctx->GetNumType(), {}, false),
		std::make_shared<NativeInst>(csaw::stdlib::random));
	runtime->RegisterNativeFunc(
		"readf",
		ctx->GetFunctionType(ctx->GetStrType(), { ctx->GetStrType() }, true),
		std::make_shared<NativeInst>(csaw::stdlib::readf));
}

int main(int argc, char** argv)
{
	std::string filename = "C:/Users/Felix Schreiber/Documents/Projects/C++/CSaw/csaw/ghost/main.csaw";
	std::ifstream stream(filename);
	if (!stream.is_open())
		std::cerr << "Failed to open stream from '" << filename << "'" << std::endl;

	ctx = std::make_shared<Context>();

	ctx->PushFilepath(filename);
	Parser::Parse(stream, callback);
	stream.close();
	ctx->PopFilepath();

	std::cout << *ctx << std::endl;

	auto runtime = std::make_shared<Runtime>(ctx);
	registerNative(runtime);
	runtime->PreStart();

	auto value = runtime->Call("main", nullptr, {});
	auto result = value->AsNum()->Value;

	return int(long(result));
}
