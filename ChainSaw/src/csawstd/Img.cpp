#include <csawstd/Img.h>
#include <runtime/Const.h>

using namespace csaw::runtime;

void csawstd::InitImg()
{
	Environment::CreateThing("img", "", { {"ptr", "str"} });
	Environment::CreateFunction(true, "img", "img", { "num", "num" }, false, "", csawstd::img);
	Environment::CreateFunction(false, "set", "", { "num", "num", "num" }, false, "img", csawstd::img_set);
	Environment::CreateFunction(false, "write", "", { "str", "str" }, false, "img", csawstd::img_write);
}

csawstd::Img* csawstd::Img::FromStr(const std::string& str)
{
	return (Img*)std::stoll(str);
}

csawstd::Img::Img(long width, long height)
	: Width(width), Height(height)
{
	Pixels = new int[Width * Height];
}

std::string csawstd::Img::ToStr()
{
	return std::to_string((intptr_t)this);
}

void csawstd::Img::Set(long x, long y, int rgba)
{
	Pixels[x + y * Width] = rgba;
}

void csawstd::Img::Write(const std::string& format, const std::string& filename)
{
	int success = 0;
	if (format == "png") success = stbi_write_png(filename.c_str(), Width, Height, 4, Pixels, 4 * Width);
	else if (format == "bmp") success = stbi_write_bmp(filename.c_str(), Width, Height, 4, Pixels);
	else if (format == "tga") success = stbi_write_tga(filename.c_str(), Width, Height, 4, Pixels);
	else if (format == "jpg") success = stbi_write_jpg(filename.c_str(), Width, Height, 4, Pixels, 90);

	if (!success)
		std::cerr << "Failed to write data in format '" << format << "' to file '" << filename << "'" << std::endl;
}

ValuePtr csawstd::img(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	auto ptr = new Img(long(args[0]->GetNum()), long(args[1]->GetNum()));
	return ConstThing::Ptr("img", { {"ptr", ConstStr::Ptr(ptr->ToStr())} });
}

ValuePtr csawstd::img_set(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	auto ptr = Img::FromStr(callee->GetElement("ptr")->GetStr());

	long x = long(args[0]->GetNum());
	long y = long(args[1]->GetNum());
	int rgba = int(args[2]->GetNum());

	ptr->Set(x, y, rgba);

	return ValuePtr();
}

ValuePtr csawstd::img_write(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	auto ptr = Img::FromStr(callee->GetElement("ptr")->GetStr());

	auto format = args[0]->GetStr();
	auto filename = args[1]->GetStr();

	ptr->Write(format, filename);

	return ValuePtr();
}
