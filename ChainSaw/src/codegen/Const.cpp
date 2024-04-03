#include <codegen/Type.h>
#include <codegen/Value.h>

#include <iostream>

csaw::codegen::Const::Const(TypePtr type, const std::string& name)
	: Value(type, name)
{
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

csaw::codegen::ConstNum::ConstNum(NumTypePtr type, double value, const std::string& name)
	: Const(type, name), Value(value)
{
}

void csaw::codegen::ConstNum::Set(ValuePtr other)
{
	if (other->Type != Type) throw;
	Value = other->AsConst()->AsNum()->Value;
}

std::ostream& csaw::codegen::ConstNum::Print(std::ostream& out) const
{
	return PrintValue(out << Type->GetName() << " ");
}

std::ostream& csaw::codegen::ConstNum::PrintValue(std::ostream& out) const
{
	return out << Value;
}

csaw::codegen::ConstChr::ConstChr(ChrTypePtr type, char value, const std::string& name)
	: Const(type, name), Value(value)
{
}

void csaw::codegen::ConstChr::Set(ValuePtr other)
{
	if (other->Type != Type) throw;
	Value = other->AsConst()->AsChr()->Value;
}

std::ostream& csaw::codegen::ConstChr::Print(std::ostream& out) const
{
	return PrintValue(out << Type->GetName() << " '") << "'";
}

std::ostream& csaw::codegen::ConstChr::PrintValue(std::ostream& out) const
{
	return out << Value;
}

csaw::codegen::ConstStr::ConstStr(StrTypePtr type, const std::string& value, const std::string& name)
	: Const(type, name), Value(value)
{
}

void csaw::codegen::ConstStr::Set(ValuePtr other)
{
	if (other->Type != Type) throw;
	Value = other->AsConst()->AsStr()->Value;
}

std::ostream& csaw::codegen::ConstStr::Print(std::ostream& out) const
{
	return PrintValue(out << Type->GetName() << " \"") << "\"";
}

std::ostream& csaw::codegen::ConstStr::PrintValue(std::ostream& out) const
{
	std::string value;
	for (auto c : Value)
		switch (c)
		{
		case '\a': value += "\\a"; break;
		case '\b': value += "\\b"; break;
		case '\f': value += "\\f"; break;
		case '\n': value += "\\n"; break;
		case '\r': value += "\\r"; break;
		case '\t': value += "\\t"; break;
		case '\v': value += "\\v"; break;
		default: value += c; break;
		}
	return out << value;
}

csaw::codegen::ConstThing::ConstThing(ThingTypePtr type, const std::map<std::string, ValuePtr>& elements, const std::string& name)
	: Const(type, name), Elements(elements)
{
}

void csaw::codegen::ConstThing::Set(ValuePtr other)
{
	if (other->Type != Type) throw;
	Elements = other->AsConst()->AsThing()->Elements;
}

std::ostream& csaw::codegen::ConstThing::Print(std::ostream& out) const
{
	return PrintValue(out << Type->GetName() << " ");
}

std::ostream& csaw::codegen::ConstThing::PrintValue(std::ostream& out) const
{
	out << "{ ";
	for (auto& elem : Elements)
		elem.second->Print(out << elem.first << " = ") << ", ";
	return out << "}";
}
