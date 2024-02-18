#include <codegen/Value.h>

csaw::codegen::Const::Const(TypePtr type)
	: Value(type)
{
}

csaw::codegen::ConstNum::ConstNum(NumTypePtr type, double value)
	: Const(type), Value(value)
{
}

csaw::codegen::ConstChr::ConstChr(ChrTypePtr type, char value)
	: Const(type), Value(value)
{
}

csaw::codegen::ConstStr::ConstStr(StrTypePtr type, const std::string& value)
	: Const(type), Value(value)
{
}

csaw::codegen::ConstThing::ConstThing(ThingTypePtr type, const std::map<std::string, ValuePtr>& elements)
	: Const(type), Elements(elements)
{
}
