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
		: std::enable_shared_from_this<Type>
	{
		virtual ~Type() {}
		virtual std::string GetName() const = 0;

		virtual bool IsNum() const;
		virtual bool IsChr() const;
		virtual bool IsStr() const;
		virtual bool IsEmpty() const;
		virtual bool IsThing() const;
		virtual bool IsFunction() const;

		NumTypePtr AsNum();
		ChrTypePtr AsChr();
		StrTypePtr AsStr();
		EmptyTypePtr AsEmpty();
		ThingTypePtr AsThing();
		FunctionTypePtr AsFunction();
	};

	struct NumType : Type
	{
		std::string GetName() const override;
		bool IsNum() const override;
	};

	struct ChrType : Type
	{
		std::string GetName() const override;
		bool IsChr() const override;
	};

	struct StrType : Type
	{
		std::string GetName() const override;
		bool IsStr() const override;
	};

	struct EmptyType : Type
	{
		std::string GetName() const override;
		bool IsEmpty() const override;
	};

	struct ThingType : Type
	{
		ThingType(const std::string& name, const std::map<std::string, TypePtr>& elements);

		std::string GetName() const override;
		bool IsThing() const override;

		std::string Name;
		std::map<std::string, TypePtr> Elements;
	};

	struct FunctionType : Type
	{
		FunctionType(TypePtr result, const std::vector<TypePtr>& argtypes, bool isvararg);

		std::string GetName() const override;
		bool IsFunction() const override;

		TypePtr Result;
		std::vector<TypePtr> ArgTypes;
		bool IsVarArg;
	};
}
