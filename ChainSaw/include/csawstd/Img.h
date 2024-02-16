#pragma once

#include <runtime/Runtime.h>

#include <stb_image_write.h>

namespace csawstd
{
	using namespace csaw::runtime;

	void InitImg();

	struct Img
	{
		static Img* FromStr(const std::string& str);

		Img(long width, long height);

		std::string ToStr();
		void Set(long x, long y, int rgba);
		void Write(const std::string& format, const std::string& filename);

		long Width;
		long Height;
		int* Pixels;
	};

	ValuePtr img(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr img_set(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
	ValuePtr img_write(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args);
}
