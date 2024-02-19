#include <codegen/Type.h>

bool csaw::codegen::Type::IsNum() const
{
	return false;
}

bool csaw::codegen::Type::IsChr() const
{
	return false;
}

bool csaw::codegen::Type::IsStr() const
{
	return false;
}

bool csaw::codegen::Type::IsEmpty() const
{
	return false;
}

bool csaw::codegen::Type::IsThing() const
{
	return false;
}

bool csaw::codegen::Type::IsFunction() const
{
	return false;
}

csaw::codegen::NumTypePtr csaw::codegen::Type::AsNum()
{
	return std::dynamic_pointer_cast<NumType>(shared_from_this());
}

csaw::codegen::ChrTypePtr csaw::codegen::Type::AsChr()
{
	return std::dynamic_pointer_cast<ChrType>(shared_from_this());
}

csaw::codegen::StrTypePtr csaw::codegen::Type::AsStr()
{
	return std::dynamic_pointer_cast<StrType>(shared_from_this());
}

csaw::codegen::EmptyTypePtr csaw::codegen::Type::AsEmpty()
{
	return std::dynamic_pointer_cast<EmptyType>(shared_from_this());
}

csaw::codegen::ThingTypePtr csaw::codegen::Type::AsThing()
{
	return std::dynamic_pointer_cast<ThingType>(shared_from_this());
}

csaw::codegen::FunctionTypePtr csaw::codegen::Type::AsFunction()
{
	return std::dynamic_pointer_cast<FunctionType>(shared_from_this());
}

std::string csaw::codegen::NumType::GetName() const
{
	return "num";
}

bool csaw::codegen::NumType::IsNum() const
{
	return true;
}

std::string csaw::codegen::ChrType::GetName() const
{
	return "chr";
}

bool csaw::codegen::ChrType::IsChr() const
{
	return true;
}

std::string csaw::codegen::StrType::GetName() const
{
	return "str";
}

bool csaw::codegen::StrType::IsStr() const
{
	return true;
}

std::string csaw::codegen::EmptyType::GetName() const
{
	return "";
}

bool csaw::codegen::EmptyType::IsEmpty() const
{
	return true;
}

csaw::codegen::ThingType::ThingType(const std::string& name, const std::map<std::string, TypePtr>& elements)
	: Name(name), Elements(elements)
{
}

std::string csaw::codegen::ThingType::GetName() const
{
	return Name;
}

bool csaw::codegen::ThingType::IsThing() const
{
	return true;
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

bool csaw::codegen::FunctionType::IsFunction() const
{
	return true;
}
