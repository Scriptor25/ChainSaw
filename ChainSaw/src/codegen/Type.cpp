#include <codegen/Type.h>

std::string csaw::codegen::NumType::GetName() const
{
	return "num";
}

std::string csaw::codegen::ChrType::GetName() const
{
	return "chr";
}

std::string csaw::codegen::StrType::GetName() const
{
	return "str";
}

std::string csaw::codegen::EmptyType::GetName() const
{
	return "";
}

csaw::codegen::ThingType::ThingType(const std::string& name, const std::map<std::string, TypePtr>& elements)
	: Name(name), Elements(elements)
{
}

std::string csaw::codegen::ThingType::GetName() const
{
	return Name;
}

csaw::codegen::FunctionType::FunctionType(TypePtr result, const std::vector<TypePtr>& argtypes, bool isvararg)
	: Result(result), ArgTypes(argtypes), IsVarArg(isvararg)
{
}

std::string csaw::codegen::FunctionType::GetName() const
{
	std::string name = Result->GetName() + '(';
	for (size_t i = 0; i < ArgTypes.size(); i++)
	{
		if (i > 0) name += ", ";
		name += ArgTypes[i]->GetName();
	}

	name += ')';

	if (IsVarArg) name += '?';

	return name;
}
