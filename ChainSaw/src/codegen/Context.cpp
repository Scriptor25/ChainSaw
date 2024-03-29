#include <codegen/Context.h>
#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

#include <iostream>

csaw::codegen::Context::Context()
{
	auto type = std::make_shared<FunctionType>(GetEmptyType(), std::vector<TypePtr>(), false);
	m_GlobalInsertPoint = std::make_shared<Function>(std::string("__global__"), type, false, std::vector<ArgPtr>(), GetEmptyType());
	SetInsertGlobal();
}

std::ostream& csaw::codegen::Context::Print() const
{
	return Print(std::cout);
}

std::ostream& csaw::codegen::Context::Print(std::ostream& out) const
{
	out << "; filepath " << Filepath() << std::endl;
	out << std::endl;

	for (auto& type : m_Types)
		if (type.second->IsThing())
			type.second->Print(out) << std::endl;

	for (auto& var : m_GlobalVariables)
		var.second->Print(out << "@" << var.first << " = ") << std::endl;

	for (auto& func : DeclaredFunctions())
		func->Print(out << std::endl) << std::endl;

	m_GlobalInsertPoint->Print(out << std::endl) << std::endl;

	for (auto& func : DefinedFunctions())
		func->Print(out << std::endl) << std::endl;

	return out;
}

csaw::codegen::TypePtr csaw::codegen::Context::GetType(const std::string& name)
{
	if (name == "num") return GetNumType();
	if (name == "chr") return GetChrType();
	if (name == "str") return GetStrType();
	if (name == "") return GetEmptyType();
	return GetThingType(name);
}

csaw::codegen::NumTypePtr csaw::codegen::Context::GetNumType()
{
	if (auto& type = m_Types["num"])
		return type->AsNum();
	auto type = std::make_shared<NumType>();
	m_Types["num"] = type;
	return type;
}

csaw::codegen::ChrTypePtr csaw::codegen::Context::GetChrType()
{
	if (auto& type = m_Types["chr"])
		return type->AsChr();
	auto type = std::make_shared<ChrType>();
	m_Types["chr"] = type;
	return type;
}

csaw::codegen::StrTypePtr csaw::codegen::Context::GetStrType()
{
	if (auto& type = m_Types["str"])
		return type->AsStr();
	auto type = std::make_shared<StrType>();
	m_Types["str"] = type;
	return type;
}

csaw::codegen::EmptyTypePtr csaw::codegen::Context::GetEmptyType()
{
	if (auto& type = m_Types[""])
		return type->AsEmpty();
	auto type = std::make_shared<EmptyType>();
	m_Types[""] = type;
	return type;
}

csaw::codegen::ThingTypePtr csaw::codegen::Context::GetThingType(const std::string& name)
{
	if (auto& type = m_Types[name])
		return type->AsThing();
	auto type = std::make_shared<ThingType>(name, std::map<std::string, TypePtr>());
	m_Types[name] = type;
	return type;
}

csaw::codegen::FunctionTypePtr csaw::codegen::Context::GetFunctionType(TypePtr result, const std::vector<TypePtr>& argtypes, bool isvararg)
{
	auto ftype = std::make_shared<FunctionType>(result, argtypes, isvararg);
	auto name = ftype->GetName();
	if (auto& type = m_Types[name])
		return type->AsFunction();
	m_Types[name] = ftype;
	return ftype;
}

csaw::codegen::FunctionPtr csaw::codegen::Context::GetFunction(const std::string& name, TypePtr callee, FunctionTypePtr type, bool isconstructor)
{
	if (auto& function = m_Functions[callee][name][type])
		if (function->IsConstructor == isconstructor)
			return function;
		else throw;

	std::vector<ArgPtr> args;
	for (size_t i = 0; i < type->ArgTypes.size(); i++)
		args.push_back(std::make_shared<Arg>(type->ArgTypes[i]));

	return m_Functions[callee][name][type] = std::make_shared<Function>(name, type, isconstructor, args, callee);
}

csaw::codegen::FunctionPtr csaw::codegen::Context::GetFunction(const std::string& name, TypePtr callee, const std::vector<TypePtr>& argtypes)
{
	auto argc = argtypes.size();

	auto& funcs = m_Functions[callee][name];
	for (auto& funcentry : funcs)
	{
		auto& type = funcentry.first;
		auto targc = type->ArgTypes.size();
		if (targc > argc || (!type->IsVarArg && targc != argc))
			continue;

		size_t i = 0;
		for (; i < targc; i++)
			if (type->ArgTypes[i] != argtypes[i])
				break;
		if (i < targc)
			continue;

		return funcentry.second;
	}

	return nullptr;
}

std::filesystem::path& csaw::codegen::Context::Filepath()
{
	return m_Filepaths.back();
}

const std::filesystem::path& csaw::codegen::Context::Filepath() const
{
	return m_Filepaths.back();
}

const std::vector<std::filesystem::path>& csaw::codegen::Context::Filepaths() const
{
	return m_Filepaths;
}

const std::vector<csaw::codegen::FunctionPtr> csaw::codegen::Context::Functions() const
{
	std::vector<FunctionPtr> functions;
	for (auto& a : m_Functions)
		for (auto& b : a.second)
			for (auto& c : b.second)
				functions.push_back(c.second);
	return functions;
}

const std::vector<csaw::codegen::FunctionPtr> csaw::codegen::Context::DefinedFunctions() const
{
	std::vector<FunctionPtr> functions;
	for (auto& a : m_Functions)
		for (auto& b : a.second)
			for (auto& c : b.second)
				if (c.second->Next)
					functions.push_back(c.second);
	return functions;
}

const std::vector<csaw::codegen::FunctionPtr> csaw::codegen::Context::DeclaredFunctions() const
{
	std::vector<FunctionPtr> functions;
	for (auto& a : m_Functions)
		for (auto& b : a.second)
			for (auto& c : b.second)
				if (!c.second->Next)
					functions.push_back(c.second);
	return functions;
}

const csaw::codegen::FunctionPtr& csaw::codegen::Context::GetGlobal() const
{
	return m_GlobalInsertPoint;
}

void csaw::codegen::Context::PushFilepath(const std::filesystem::path& filepath)
{
	m_Filepaths.push_back(filepath);
}

void csaw::codegen::Context::PopFilepath()
{
	m_Filepaths.pop_back();
}

void csaw::codegen::Context::ClearVariables()
{
	m_Variables.clear();
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateVariable(const std::string& name, TypePtr type)
{
	if (!type) throw;
	auto value = std::make_shared<Value>(type);
	if (m_InsertPoint == m_GlobalInsertPoint)
	{
		if (m_GlobalVariables[name]) throw;
		m_GlobalVariables[name] = value;
		return value;
	}
	if (m_Variables[name]) throw;
	m_Variables[name] = value;
	return value;
}

csaw::codegen::ValuePtr csaw::codegen::Context::GetVariable(const std::string& name)
{
	if (m_InsertPoint == m_GlobalInsertPoint)
	{
		if (!m_GlobalVariables[name]) throw;
		return m_GlobalVariables[name];
	}
	if (!m_Variables[name])
	{
		if (!m_GlobalVariables[name]) throw;
		return m_GlobalVariables[name];
	}
	return m_Variables[name];
}

void csaw::codegen::Context::CreateArgs(const std::vector<ArgPtr>& args)
{
	for (auto& arg : args)
		m_Variables[arg->Name] = arg;
}
