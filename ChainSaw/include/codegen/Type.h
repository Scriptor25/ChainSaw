#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace csaw::codegen
{
	struct Type;
	struct NumType;
	struct ChrType;
	struct StrType;
	struct EmptyType;
	struct ThingType;
	struct FunctionType;

	typedef std::shared_ptr<Type> TypePtr;
	typedef std::shared_ptr<NumType> NumTypePtr;
	typedef std::shared_ptr<ChrType> ChrTypePtr;
	typedef std::shared_ptr<StrType> StrTypePtr;
	typedef std::shared_ptr<EmptyType> EmptyTypePtr;
	typedef std::shared_ptr<ThingType> ThingTypePtr;
	typedef std::shared_ptr<FunctionType> FunctionTypePtr;

	struct Type
	{
		virtual ~Type() {}
		virtual std::string GetName() const = 0;
	};

	struct NumType : Type
	{
		std::string GetName() const override;
	};

	struct ChrType : Type
	{
		std::string GetName() const override;
	};

	struct StrType : Type
	{
		std::string GetName() const override;
	};

	struct EmptyType : Type
	{
		std::string GetName() const override;
	};

	struct ThingType : Type
	{
		ThingType(const std::string& name, const std::map<std::string, TypePtr>& elements);

		std::string GetName() const override;

		std::string Name;
		std::map<std::string, TypePtr> Elements;
	};

	struct FunctionType : Type
	{
		FunctionType(TypePtr result, const std::vector<TypePtr>& argtypes, bool isvararg);

		std::string GetName() const override;

		TypePtr Result;
		std::vector<TypePtr> ArgTypes;
		bool IsVarArg;
	};
}
