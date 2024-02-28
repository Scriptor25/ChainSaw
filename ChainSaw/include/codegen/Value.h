#pragma once

#include <codegen/Def.h>

#include <map>
#include <string>

namespace csaw::codegen
{
	struct ValueRef
	{
		ValueRef(TypePtr type, const std::string& name = "");
		ValueRef(ValuePtr value);

		TypePtr Type(TypePtr def = nullptr) const;

		std::ostream& Print(std::ostream& out) const;

		ValuePtr Value;
	};

	struct Value
		: std::enable_shared_from_this<Value>
	{
		static ValuePtr Default(TypePtr type, const std::string& name = "");
		static std::string CreateName();

		Value(TypePtr type, const std::string& name = "");
		virtual ~Value() {}

		std::ostream& Print() const;
		virtual std::ostream& Print(std::ostream& out) const;

		ConstPtr AsConst();

		std::string Name;
		TypePtr Type;
	};

	struct Const : Value
	{
		static ConstPtr Default(TypePtr type, const std::string& name = "");

		Const(TypePtr type, const std::string& name = "");

		virtual std::ostream& PrintValue(std::ostream& out) const = 0;

		ConstNumPtr AsNum();
		ConstChrPtr AsChr();
		ConstStrPtr AsStr();
		ConstThingPtr AsThing();
	};

	struct ConstNum : Const
	{
		ConstNum(NumTypePtr type, double value, const std::string& name = "");

		std::ostream& Print(std::ostream& out) const override;
		std::ostream& PrintValue(std::ostream& out) const override;

		double Value;
	};

	struct ConstChr : Const
	{
		ConstChr(ChrTypePtr type, char value, const std::string& name = "");

		std::ostream& Print(std::ostream& out) const override;
		std::ostream& PrintValue(std::ostream& out) const override;

		char Value;
	};

	struct ConstStr : Const
	{
		ConstStr(StrTypePtr type, const std::string& value, const std::string& name = "");

		std::ostream& Print(std::ostream& out) const override;
		std::ostream& PrintValue(std::ostream& out) const override;

		std::string Value;
	};

	struct ConstThing : Const
	{
		ConstThing(ThingTypePtr type, const std::map<std::string, ValuePtr>& elements, const std::string& name = "");

		std::ostream& Print(std::ostream& out) const override;
		std::ostream& PrintValue(std::ostream& out) const override;

		std::map<std::string, ValuePtr> Elements;
	};
}
