//#define PRINT_PARSE

#include <lang/Parser.h>

#include <codegen/CodeGen.h>
#include <codegen/Context.h>
#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

#include <runtime/Runtime.h>

#include <misc/stb_image_write.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

csaw::codegen::ContextPtr ctx;
csaw::runtime::RuntimePtr runtime;

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
	struct Img
	{
		static Img* FromString(const std::string& str)
		{
			return (Img*)std::stoll(str);
		}

		Img(int w, int h)
			: Width(w), Height(h)
		{
			Pixels = new uint32_t[w * h];
		}

		std::string ToString()
		{
			return std::to_string((intptr_t)this);
		}

		void Set(int x, int y, uint32_t argb)
		{
			Pixels[x + y * Width] = argb;
		}

		void Write(const std::string& fmt, const std::string& filename)
		{
			int success = 0;
			if (fmt == "bmp") success = stbi_write_bmp(filename.c_str(), Width, Height, 4, Pixels);
			if (fmt == "jpg") success = stbi_write_jpg(filename.c_str(), Width, Height, 4, Pixels, 90);
			if (fmt == "png") success = stbi_write_png(filename.c_str(), Width, Height, 4, Pixels, 4 * Width);
			if (fmt == "tga") success = stbi_write_tga(filename.c_str(), Width, Height, 4, Pixels);

			if (!success) throw;
		}

		int Width, Height;
		uint32_t* Pixels;
	};

	static ValuePtr floor(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		auto x = args[0]->AsConst()->AsNum();
		return context->GetConstNum(std::floor(x->Value));
	}

	static ValuePtr num(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		auto x = args[0]->AsConst()->AsStr();
		return context->GetConstNum(std::stod(x->Value));
	}

	static void formatstream(std::ostream& stream, const std::string& fmtsrc, const std::vector<ValuePtr>& args, size_t offset)
	{
		std::string fmt = fmtsrc;
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
			if (i >= offset)
				args[i]->AsConst()->PrintValue(stream);
			stream << format[i];
		}
	}

	static ValuePtr format(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		std::stringstream stream;
		formatstream(stream, args[0]->AsConst()->AsStr()->Value, args, 1);
		return context->GetConstStr(stream.str());
	}

	static ValuePtr printf(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		formatstream(std::cout, args[0]->AsConst()->AsStr()->Value, args, 1);
		return nullptr;
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

	static ValuePtr random(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		double r = UDIST(RNG);
		return context->GetConstNum(r);
	}

	static ValuePtr readf(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		printf(context, callee, args);

		std::string line;
		std::getline(std::cin, line);

		return context->GetConstStr(line);
	}

	static ValuePtr atan2(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		auto y = args[0]->AsConst()->AsNum();
		auto x = args[1]->AsConst()->AsNum();
		return context->GetConstNum(std::atan2(y->Value, x->Value));
	}

	static ValuePtr cos(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		auto x = args[0]->AsConst()->AsNum();
		return context->GetConstNum(std::cos(x->Value));
	}

	static ValuePtr exp(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		auto x = args[0]->AsConst()->AsNum();
		return context->GetConstNum(std::exp(x->Value));
	}

	static ValuePtr img(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		auto w = args[0]->AsConst()->AsNum();
		auto h = args[1]->AsConst()->AsNum();
		auto ptr = new Img(int(w->Value), int(h->Value));
		auto ptrstr = context->GetConstStr(ptr->ToString());
		return context->GetConstThing("img", { {"ptr", ptrstr} });
	}

	static ValuePtr pow(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		auto b = args[0]->AsConst()->AsNum();
		auto p = args[1]->AsConst()->AsNum();
		return context->GetConstNum(std::pow(b->Value, p->Value));
	}

	static ValuePtr sin(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		auto x = args[0]->AsConst()->AsNum();
		return context->GetConstNum(std::sin(x->Value));
	}

	static ValuePtr sqrt(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		auto x = args[0]->AsConst()->AsNum();
		return context->GetConstNum(std::sqrt(x->Value));
	}

	static ValuePtr set(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		auto ptr = Img::FromString(callee->AsConst()->AsThing()->Elements["ptr"]->AsConst()->AsStr()->Value);

		auto x = args[0]->AsConst()->AsNum();
		auto y = args[1]->AsConst()->AsNum();
		auto argb = args[2]->AsConst()->AsNum();

		ptr->Set(int(x->Value), int(y->Value), uint32_t(argb->Value));

		return nullptr;
	}

	static ValuePtr write(ContextPtr context, ValuePtr callee, const std::vector<ValuePtr>& args)
	{
		auto ptr = Img::FromString(callee->AsConst()->AsThing()->Elements["ptr"]->AsConst()->AsStr()->Value);

		auto fmt = args[0]->AsConst()->AsStr();
		auto filename = args[1]->AsConst()->AsStr();

		ptr->Write(fmt->Value, filename->Value);

		return nullptr;
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
	runtime->RegisterNativeFunc(
		"atan2",
		ctx->GetFunctionType(ctx->GetNumType(), { ctx->GetNumType(), ctx->GetNumType() }, false),
		std::make_shared<NativeInst>(csaw::stdlib::atan2));
	runtime->RegisterNativeFunc(
		"cos",
		ctx->GetFunctionType(ctx->GetNumType(), { ctx->GetNumType() }, false),
		std::make_shared<NativeInst>(csaw::stdlib::cos));
	runtime->RegisterNativeFunc(
		"exp",
		ctx->GetFunctionType(ctx->GetNumType(), { ctx->GetNumType() }, false),
		std::make_shared<NativeInst>(csaw::stdlib::exp));
	runtime->RegisterNativeFunc(
		"img",
		ctx->GetFunctionType(ctx->GetThingType("img"), { ctx->GetNumType(), ctx->GetNumType() }, false),
		std::make_shared<NativeInst>(csaw::stdlib::img));
	runtime->RegisterNativeFunc(
		"pow",
		ctx->GetFunctionType(ctx->GetNumType(), { ctx->GetNumType(), ctx->GetNumType() }, false),
		std::make_shared<NativeInst>(csaw::stdlib::pow));
	runtime->RegisterNativeFunc(
		"sin",
		ctx->GetFunctionType(ctx->GetNumType(), { ctx->GetNumType() }, false),
		std::make_shared<NativeInst>(csaw::stdlib::sin));
	runtime->RegisterNativeFunc(
		"sqrt",
		ctx->GetFunctionType(ctx->GetNumType(), { ctx->GetNumType() }, false),
		std::make_shared<NativeInst>(csaw::stdlib::sqrt));
	runtime->RegisterNativeFunc(
		"set",
		ctx->GetFunctionType(ctx->GetEmptyType(), { ctx->GetNumType(), ctx->GetNumType(), ctx->GetNumType() }, false),
		std::make_shared<NativeInst>(csaw::stdlib::set));
	runtime->RegisterNativeFunc(
		"write",
		ctx->GetFunctionType(ctx->GetEmptyType(), { ctx->GetStrType(), ctx->GetStrType() }, false),
		std::make_shared<NativeInst>(csaw::stdlib::write));
	runtime->RegisterNativeFunc(
		"format",
		ctx->GetFunctionType(ctx->GetStrType(), { ctx->GetStrType() }, true),
		std::make_shared<NativeInst>(csaw::stdlib::format));
}

int main(int argc, char** argv)
{
	std::filesystem::path filepath = std::filesystem::absolute("../../../../csaw/mandel/main.csaw");
	std::ifstream stream(filepath);
	if (!stream.is_open())
		std::cerr << "Failed to open stream from '" << filepath << "'" << std::endl;

	ctx = std::make_shared<Context>();

	ctx->PushFilepath(filepath);
	Parser::Parse(stream, callback);
	stream.close();

	ctx->Print() << std::endl;

	runtime = std::make_shared<Runtime>(ctx);
	registerNative(runtime);
	if (!runtime->PreStart()) return 1;

	auto value = runtime->Call("main", nullptr, {});
	auto result = value->AsConst()->AsNum()->Value;

	return int(long(result));
}
