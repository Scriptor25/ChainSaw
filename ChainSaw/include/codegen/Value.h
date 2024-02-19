#pragma once

#include <codegen/Type.h>

#include <map>
#include <memory>
#include <string>

namespace csaw::codegen
{
	struct Value;

	struct Const;
	struct ConstNum;
	struct ConstChr;
	struct ConstStr;
	struct ConstThing;

	typedef std::shared_ptr<Value> ValuePtr;

	typedef std::shared_ptr<Const> ConstPtr;
	typedef std::shared_ptr<ConstNum> ConstNumPtr;
	typedef std::shared_ptr<ConstChr> ConstChrPtr;
	typedef std::shared_ptr<ConstStr> ConstStrPtr;
	typedef std::shared_ptr<ConstThing> ConstThingPtr;

	struct Value
		: std::enable_shared_from_this<Value>
	{
		static ValuePtr Default(TypePtr type);

		Value(TypePtr type);
		virtual ~Value() {}

		TypePtr Type;
	};

	struct Const : Value
	{
		static ConstPtr Default(TypePtr type);

		ConstNumPtr AsNum();
		ConstChrPtr AsChr();
		ConstStrPtr AsStr();
		ConstThingPtr AsThing();

		Const(TypePtr type);
	};

	struct ConstNum : Const
	{
		ConstNum(NumTypePtr type, double value);

		double Value;
	};

	struct ConstChr : Const
	{
		ConstChr(ChrTypePtr type, char value);

		char Value;
	};

	struct ConstStr : Const
	{
		ConstStr(StrTypePtr type, const std::string& value);

		std::string Value;
	};

	struct ConstThing : Const
	{
		ConstThing(ThingTypePtr type, const std::map<std::string, ValuePtr>& elements);

		std::map<std::string, ValuePtr> Elements;
	};
}
