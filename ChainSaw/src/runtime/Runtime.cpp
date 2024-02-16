#include <runtime/Runtime.h>
#include <runtime/Const.h>

#include <cassert>

std::ostream& csaw::runtime::operator<<(std::ostream& out, const ValuePtr& ptr)
{
	if (auto p = std::dynamic_pointer_cast<ConstNum>(ptr))
		return out << *p;
	if (auto p = std::dynamic_pointer_cast<ConstChr>(ptr))
		return out << *p;
	if (auto p = std::dynamic_pointer_cast<ConstStr>(ptr))
		return out << *p;

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::Value::DefaultPtr(const std::string& type)
{
	if (type == "num")
		return ConstNum::Ptr(0.0);
	if (type == "chr")
		return ConstChr::Ptr(0);
	if (type == "str")
		return ConstStr::Ptr("");

	auto& elements = Environment::GetThing(type);
	if (!elements.empty())
	{
		std::map<std::string, ValuePtr> values;
		for (auto& e : elements)
			values[e.first] = DefaultPtr(e.second);
		return ConstThing::Ptr(type, values);
	}

	throw;
}

double csaw::runtime::Value::GetNum() const
{
	throw;
}

char csaw::runtime::Value::GetChr() const
{
	throw;
}

std::string csaw::runtime::Value::GetStr() const
{
	throw;
}

csaw::runtime::ValuePtr csaw::runtime::Value::GetElement(const std::string& name)
{
	throw;
}

csaw::runtime::ValuePtr csaw::runtime::Value::SetElement(const std::string& name, const ValuePtr value)
{
	throw;
}

bool csaw::runtime::Value::IsReturned() const
{
	return Returned;
}

void csaw::runtime::Value::MarkReturned(bool b)
{
	Returned = b;
}

csaw::runtime::VariablePtr csaw::runtime::Variable::Ptr(const std::string& type, ValuePtr value)
{
	if (!value)
		value = Value::DefaultPtr(type);
	assert(type == value->GetType());
	return std::make_shared<Variable>(type, value);
}

csaw::runtime::Variable::Variable(const std::string& type, ValuePtr value)
	: Type(type), Value(value)
{
}

csaw::runtime::FunctionPtr csaw::runtime::Function::Ptr(const bool constructor, const std::string& result, const std::vector<std::string>& params, const bool vararg, const FunDefinition& definition)
{
	return std::make_shared<Function>(constructor, result, params, vararg, definition);
}

csaw::runtime::Function::Function(const bool constructor, const std::string& result, const std::vector<std::string>& params, const bool vararg, const FunDefinition& definition)
	: Constructor(constructor), Result(result), Params(params), VarArg(vararg), Definition(definition)
{
}

csaw::runtime::EnvironmentPtr csaw::runtime::Environment::Ptr(EnvironmentPtr parent)
{
	return std::make_shared<Environment>(parent->m_Filepath, parent);
}

csaw::runtime::EnvironmentPtr csaw::runtime::Environment::Ptr(const std::string& filepath)
{
	return std::make_shared<Environment>(filepath, EnvironmentPtr());
}

void csaw::runtime::Environment::CreateFunction(const bool constructor, const std::string& name, const std::string& result, const std::vector<std::string>& params, const bool vararg, const std::string& callee, const FunDefinition& definition)
{
	_Functions[callee][name].push_back(Function::Ptr(constructor, result, params, vararg, definition));
}

csaw::runtime::FunctionPtr csaw::runtime::Environment::GetFunction(const std::string& name, const std::string& callee, const std::vector<std::string>& args)
{
	auto as = args.size();
	auto& functions = _Functions[callee][name];
	for (auto& function : functions)
	{
		auto fps = function->Params.size();
		if (fps > as || (!function->VarArg && fps != as))
			continue;

		size_t i = 0;
		for (; i < fps; i++)
			if (function->Params[i] != args[i])
				break;
		if (i < fps)
			continue;

		return function;
	}

	return FunctionPtr();
}

csaw::runtime::ValuePtr csaw::runtime::Environment::GetAndInvoke(EnvironmentPtr env, const std::string& name, const ValuePtr callee, const std::vector<ValuePtr>& args)
{
	std::string calleet = callee ? callee->GetType() : "";
	std::vector<std::string> argts;
	for (auto& arg : args)
		argts.push_back(arg->GetType());
	auto function = GetFunction(name, calleet, argts);
	assert(function);
	return function->Definition(env, callee, args);
}

void csaw::runtime::Environment::CreateThing(const std::string& name, const std::vector<std::pair<std::string, std::string>>& elements)
{
	_Things[name] = elements;
}

std::vector<std::pair<std::string, std::string>>& csaw::runtime::Environment::GetThing(const std::string& name)
{
	return _Things[name];
}

std::map<std::string, std::map<std::string, std::vector<csaw::runtime::FunctionPtr>>> csaw::runtime::Environment::_Functions;
std::map<std::string, std::vector<std::pair<std::string, std::string>>> csaw::runtime::Environment::_Things;

csaw::runtime::Environment::Environment(const std::string& filepath, EnvironmentPtr parent)
	: m_Filepath(filepath), m_Parent(parent)
{
}

csaw::runtime::EnvironmentPtr csaw::runtime::Environment::GetGlobal()
{
	if (m_Parent)
		return m_Parent->GetGlobal();
	return shared_from_this();
}

std::string csaw::runtime::Environment::GetFilepath()
{
	return m_Filepath;
}

void csaw::runtime::Environment::SetFilepath(const std::string& filepath)
{
	m_Filepath = filepath;
}

csaw::runtime::ValuePtr csaw::runtime::Environment::CreateVariable(const std::string& name, const std::string& type, const ValuePtr value)
{
	assert(!GetVarEntry(name));
	m_Variables[name] = Variable::Ptr(type, value);
	return value;
}

csaw::runtime::ValuePtr csaw::runtime::Environment::GetVariable(const std::string& name)
{
	auto entry = GetVarEntry(name);
	assert(entry);
	return entry->Value;
}

csaw::runtime::ValuePtr csaw::runtime::Environment::SetVariable(const std::string& name, const ValuePtr value)
{
	auto entry = GetVarEntry(name);
	assert(entry);
	assert(entry->Type == value->GetType());
	return entry->Value = value;
}

void csaw::runtime::Environment::PushVarArg(const ValuePtr value)
{
	m_VarArgs.push_back(value);
}

csaw::runtime::ValuePtr csaw::runtime::Environment::GetVarArg(size_t index, const std::string& type)
{
	if (!m_VarArgs.empty())
	{
		auto& value = m_VarArgs[index];
		assert(value && (value->GetType() == type));
		return value;
	}
	if (!m_Parent)
		return ValuePtr();
	return m_Parent->GetVarArg(index, type);
}

csaw::runtime::VariablePtr csaw::runtime::Environment::GetVarEntry(const std::string& name)
{
	if (auto& entry = m_Variables[name])
		return entry;
	if (!m_Parent)
		return VariablePtr();
	return m_Parent->GetVarEntry(name);
}
