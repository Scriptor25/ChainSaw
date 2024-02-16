#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace csaw::runtime
{
	struct Value;
	struct Variable;
	struct Function;

	typedef std::shared_ptr<Value> ValuePtr;
	typedef std::shared_ptr<Variable> VariablePtr;
	typedef std::shared_ptr<Function> FunctionPtr;

	std::ostream& operator<<(std::ostream& out, const ValuePtr& ptr);

	class Environment;
	typedef std::shared_ptr<Environment> EnvironmentPtr;

	struct Value
	{
		static ValuePtr DefaultPtr(const std::string& type);

		virtual ~Value() {}
		virtual std::string GetType() const = 0;

		virtual double GetNum() const;
		virtual char GetChr() const;
		virtual std::string GetStr() const;
		virtual ValuePtr GetElement(const std::string& name);
		virtual ValuePtr SetElement(const std::string& name, const ValuePtr value);

		bool IsReturned() const;
		void MarkReturned(bool b);

		bool Returned = false;
	};

	struct Variable
	{
		static VariablePtr Ptr(const std::string& type, ValuePtr value);
		Variable(const std::string& type, ValuePtr value);

		const std::string Type;
		ValuePtr Value;
	};

	typedef std::function<ValuePtr(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)> FunDefinition;

	struct Function
	{
		static FunctionPtr Ptr(const bool constructor, const std::string& result, const std::vector<std::string>& params, const bool vararg, const FunDefinition& definition);
		Function(const bool constructor, const std::string& result, const std::vector<std::string>& params, const bool vararg, const FunDefinition& definition);

		const bool Constructor;
		const std::string Result;
		const std::vector<std::string> Params;
		const bool VarArg;

		FunDefinition Definition;
	};

	class Environment
		: public std::enable_shared_from_this<Environment>
	{
	public:
		static EnvironmentPtr Ptr(EnvironmentPtr parent);
		static EnvironmentPtr Ptr(const std::string& filepath);

		static void CreateFunction(const bool constructor, const std::string& name, const std::string& result, const std::vector<std::string>& params, const bool vararg, const std::string& callee, const FunDefinition& definition);
		static FunctionPtr GetFunction(const std::string& name, const std::string& callee, const std::vector<std::string>& args);
		static ValuePtr GetAndInvoke(EnvironmentPtr env, const std::string& name, const ValuePtr callee, const std::vector<ValuePtr>& args);

		static void CreateThing(const std::string& name, const std::vector<std::pair<std::string, std::string>>& elements);
		static std::vector<std::pair<std::string, std::string>>& GetThing(const std::string& name);

	private:
		static std::map<std::string, std::map<std::string, std::vector<FunctionPtr>>> _Functions;
		static std::map<std::string, std::vector<std::pair<std::string, std::string>>> _Things;

	public:
		Environment(const std::string& filepath, EnvironmentPtr parent);

		EnvironmentPtr GetGlobal();

		std::string GetFilepath();
		void SetFilepath(const std::string& filepath);

		ValuePtr CreateVariable(const std::string& name, const std::string& type, const ValuePtr value);
		ValuePtr GetVariable(const std::string& name);
		ValuePtr SetVariable(const std::string& name, const ValuePtr value);

		void PushVarArg(const ValuePtr value);
		ValuePtr GetVarArg(size_t index, const std::string& type);

	private:
		VariablePtr GetVarEntry(const std::string& name);

	private:
		EnvironmentPtr m_Parent;
		std::string m_Filepath;

		std::vector<ValuePtr> m_VarArgs;
		std::map<std::string, VariablePtr> m_Variables;
	};
}
