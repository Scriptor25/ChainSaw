#pragma once

#include <runtime/Runtime.h>

#include <iostream>

namespace csaw::runtime
{
	struct ConstNum;
	struct ConstChr;
	struct ConstStr;
	struct ConstThing;
	struct ConstUndef;

	typedef std::shared_ptr<ConstNum> ConstNumPtr;
	typedef std::shared_ptr<ConstChr> ConstChrPtr;
	typedef std::shared_ptr<ConstStr> ConstStrPtr;
	typedef std::shared_ptr<ConstThing> ConstThingPtr;
	typedef std::shared_ptr<ConstUndef> ConstUndefPtr;

	std::ostream& operator<<(std::ostream& out, const ConstNum& c);
	std::ostream& operator<<(std::ostream& out, const ConstChr& c);
	std::ostream& operator<<(std::ostream& out, const ConstStr& c);

	struct ConstNum : Value
	{
		static ConstNumPtr Ptr(const double value);
		ConstNum(const double value);

		std::string GetType() const override;
		double GetNum() const override;

		const double Value;
	};

	struct ConstChr : Value
	{
		static ConstChrPtr Ptr(const char value);
		ConstChr(const char value);

		std::string GetType() const override;
		char GetChr() const override;

		const char Value;
	};

	struct ConstStr : Value
	{
		static ConstStrPtr Ptr(const std::string& value);
		ConstStr(const std::string& value);

		std::string GetType() const override;
		std::string GetStr() const override;

		const std::string Value;
	};

	struct ConstThing : Value
	{
		static ConstThingPtr Ptr(const std::string& type, const std::map<std::string, ValuePtr>& values);
		ConstThing(const std::string& type, const std::map<std::string, ValuePtr>& values);

		std::string GetType() const override;
		ValuePtr GetElement(const std::string& name) override;
		ValuePtr SetElement(const std::string& name, const ValuePtr value) override;

		const std::string Type;
		std::map<std::string, ValuePtr> Elements;
	};

	struct ConstUndef : Value
	{
		static ConstUndefPtr Ptr(const std::string& type);
		ConstUndef(const std::string& type);

		std::string GetType() const override;

		const std::string Type;
	};
}
