#include <runtime/Const.h>

#include <cassert>

std::ostream& csaw::runtime::operator<<(std::ostream& out, const ConstNum& c)
{
	return out << c.Value;
}

std::ostream& csaw::runtime::operator<<(std::ostream& out, const ConstChr& c)
{
	return out << c.Value;
}

std::ostream& csaw::runtime::operator<<(std::ostream& out, const ConstStr& c)
{
	return out << c.Value;
}

csaw::runtime::ConstNumPtr csaw::runtime::ConstNum::Ptr(const double value)
{
	return std::make_shared<ConstNum>(value);
}

csaw::runtime::ConstNum::ConstNum(const double value)
	: Value(value)
{
}

std::string csaw::runtime::ConstNum::GetType() const
{
	return "num";
}

double csaw::runtime::ConstNum::GetNum() const
{
	return Value;
}

csaw::runtime::ConstChrPtr csaw::runtime::ConstChr::Ptr(const char value)
{
	return std::make_shared<ConstChr>(value);
}

csaw::runtime::ConstChr::ConstChr(const char value)
	: Value(value)
{
}

std::string csaw::runtime::ConstChr::GetType() const
{
	return "chr";
}

char csaw::runtime::ConstChr::GetChr() const
{
	return Value;
}

csaw::runtime::ConstStrPtr csaw::runtime::ConstStr::Ptr(const std::string& value)
{
	return std::make_shared<ConstStr>(value);
}

csaw::runtime::ConstStr::ConstStr(const std::string& value)
	: Value(value)
{
}

std::string csaw::runtime::ConstStr::GetType() const
{
	return "str";
}

std::string csaw::runtime::ConstStr::GetStr() const
{
	return Value;
}

csaw::runtime::ConstThingPtr csaw::runtime::ConstThing::Ptr(const std::string& type, const std::map<std::string, ValuePtr>& values)
{
	return std::make_shared<ConstThing>(type, values);
}

csaw::runtime::ConstThing::ConstThing(const std::string& type, const std::map<std::string, ValuePtr>& values)
	: Type(type), Elements(values)
{
}

std::string csaw::runtime::ConstThing::GetType() const
{
	return Type;
}

csaw::runtime::ValuePtr csaw::runtime::ConstThing::GetElement(const std::string& name)
{
	auto& elem = Elements[name];
	assert(elem);
	return elem;
}

csaw::runtime::ValuePtr csaw::runtime::ConstThing::SetElement(const std::string& name, const ValuePtr value)
{
	auto& elem = Elements[name];
	assert(elem);
	return elem = value;
}
