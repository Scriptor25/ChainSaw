#include <codegen/Value.h>

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
