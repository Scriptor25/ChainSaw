#include <codegen/Value.h>

#include <iostream>

csaw::codegen::ValueRef::ValueRef(TypePtr type, const std::string& name)
	: ValueRef(Value::Default(type, name))
{
}

csaw::codegen::ValueRef::ValueRef(ValuePtr value)
	: Value(value)
{
}

csaw::codegen::TypePtr csaw::codegen::ValueRef::Type(TypePtr def) const
{
	return Value ? Value->Type : def;
}

std::ostream& csaw::codegen::ValueRef::Print(std::ostream& out) const
{
	if (Value) return Value->Print(out);
	return out << "empty";
}

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

csaw::codegen::ConstPtr csaw::codegen::Value::AsConst()
{
	return std::dynamic_pointer_cast<Const>(shared_from_this());
}
