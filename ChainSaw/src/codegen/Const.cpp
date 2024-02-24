#include <codegen/Value.h>

#include <iostream>

csaw::codegen::ConstPtr csaw::codegen::Const::Default(TypePtr type)
{
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

csaw::codegen::ConstNumPtr csaw::codegen::Const::AsNum()
{
	return std::dynamic_pointer_cast<ConstNum>(shared_from_this());
}

csaw::codegen::ConstChrPtr csaw::codegen::Const::AsChr()
{
	return std::dynamic_pointer_cast<ConstChr>(shared_from_this());
}

csaw::codegen::ConstStrPtr csaw::codegen::Const::AsStr()
{
	return std::dynamic_pointer_cast<ConstStr>(shared_from_this());
}

csaw::codegen::ConstThingPtr csaw::codegen::Const::AsThing()
{
	return std::dynamic_pointer_cast<ConstThing>(shared_from_this());
}

csaw::codegen::Const::Const(TypePtr type)
	: Value(type)
{
}

csaw::codegen::ConstNum::ConstNum(NumTypePtr type, double value)
	: Const(type), Value(value)
{
}

std::ostream& csaw::codegen::ConstNum::Print(std::ostream& out) const
{
	return out << Type->GetName() << " " << Value;
}

csaw::codegen::ConstChr::ConstChr(ChrTypePtr type, char value)
	: Const(type), Value(value)
{
}

std::ostream& csaw::codegen::ConstChr::Print(std::ostream& out) const
{
	return out << Type->GetName() << " " << Value;
}

csaw::codegen::ConstStr::ConstStr(StrTypePtr type, const std::string& value)
	: Const(type), Value(value)
{
}

std::ostream& csaw::codegen::ConstStr::Print(std::ostream& out) const
{
	return out << Type->GetName() << " " << Value;
}

csaw::codegen::ConstThing::ConstThing(ThingTypePtr type, const std::map<std::string, ValuePtr>& elements)
	: Const(type), Elements(elements)
{
}

std::ostream& csaw::codegen::ConstThing::Print(std::ostream& out) const
{
	out << Type->GetName();
	for (auto& elem : Elements)
	{
		out << std::endl << "\t" << elem.first << ": ";
		elem.second->Print(out);
	}
	return out;
}
