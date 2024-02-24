#include <codegen/Value.h>

#include <iostream>

csaw::codegen::ValuePtr csaw::codegen::Value::Default(TypePtr type)
{
	if (type->IsEmpty())
		return std::make_shared<Value>(type);
	if (type->IsNum())
		return std::make_shared<ConstNum>(type->AsNum(), 0.0);
	if (type->IsChr())
		return std::make_shared<ConstChr>(type->AsChr(), '\0');
	if (type->IsStr())
		return std::make_shared<ConstStr>(type->AsStr(), "");
	if (type->IsThing())
	{
		auto thingty = type->AsThing();
		std::map<std::string, ValuePtr> elements;
		for (auto& entry : thingty->Elements)
			elements[entry.first] = Default(entry.second);
		return std::make_shared<ConstThing>(thingty, elements);
	}

	throw;
}

csaw::codegen::Value::Value(TypePtr type)
	: Type(type)
{
}

std::ostream& csaw::codegen::Value::Print() const
{
	return Print(std::cout);
}

std::ostream& csaw::codegen::Value::Print(std::ostream& out) const
{
	return out << Type->GetName();
}
