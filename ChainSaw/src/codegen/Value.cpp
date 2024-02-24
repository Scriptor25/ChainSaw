#include <codegen/Value.h>

#include <iostream>

csaw::codegen::ValuePtr csaw::codegen::Value::Default(TypePtr type, const std::string& name)
{
	std::string n = name;
	if (n.empty())
		n = CreateName();

	if (type->IsEmpty())
		return std::make_shared<Value>(type, n);
	if (type->IsNum())
		return std::make_shared<ConstNum>(type->AsNum(), 0.0, n);
	if (type->IsChr())
		return std::make_shared<ConstChr>(type->AsChr(), '\0', n);
	if (type->IsStr())
		return std::make_shared<ConstStr>(type->AsStr(), "", n);
	if (type->IsThing())
	{
		auto thingty = type->AsThing();
		std::map<std::string, ValuePtr> elements;
		for (auto& entry : thingty->Elements)
			elements[entry.first] = Default(entry.second, n + "." + entry.first);
		return std::make_shared<ConstThing>(thingty, elements, n);
	}

	throw;
}

std::string csaw::codegen::Value::CreateName()
{
	static size_t i = 0;
	return std::to_string(i++);
}

csaw::codegen::Value::Value(TypePtr type, const std::string& name)
	: Name(name.empty() ? CreateName() : name), Type(type)
{
}

std::ostream& csaw::codegen::Value::Print() const
{
	return Print(std::cout);
}

std::ostream& csaw::codegen::Value::Print(std::ostream& out) const
{
	if (!Type->IsEmpty()) out << Type->GetName() << " ";
	return out << "%" << Name;
}
