//#define PRINT_PARSE

#include <lang/Parser.h>

#include <codegen/Context.h>
#include <codegen/GenStmt.h>

#include <runtime/Runtime.h>

#include <fstream>
#include <iostream>

csaw::codegen::ContextPtr ctx;

static void callback(csaw::lang::StmtPtr stmt)
{
#ifdef PRINT_PARSE
	std::cout << stmt << std::endl;
#endif
	csaw::codegen::CodeGen(ctx, stmt);
}

int main(int argc, char** argv)
{
	std::string filename = "C:/Users/Felix Schreiber/Documents/Projects/C++/CSaw/csaw/fib/main.csaw";
	std::ifstream stream(filename);
	if (!stream.is_open())
		std::cerr << "Failed to open stream from '" << filename << "'" << std::endl;

	ctx = std::make_shared<csaw::codegen::Context>();

	ctx->PushFilepath(filename);
	csaw::lang::Parser::Parse(stream, callback);
	stream.close();
	ctx->PopFilepath();

	std::cout << *ctx << std::endl;

	csaw::runtime::Runtime runtime(ctx);

	auto value = runtime.Call("main", nullptr, {});
	auto result = std::dynamic_pointer_cast<csaw::codegen::ConstNum>(value)->Value;

	return int(long(result));
}
