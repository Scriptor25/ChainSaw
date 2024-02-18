#include <codegen/Assert.h>
#include <codegen/Context.h>

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
		return std::dynamic_pointer_cast<NumType>(type);
	auto type = std::make_shared<NumType>();
	m_Types["num"] = type;
	return type;
}

csaw::codegen::ChrTypePtr csaw::codegen::Context::GetChrType()
{
	if (auto& type = m_Types["chr"])
		return std::dynamic_pointer_cast<ChrType>(type);
	auto type = std::make_shared<ChrType>();
	m_Types["chr"] = type;
	return type;
}

csaw::codegen::StrTypePtr csaw::codegen::Context::GetStrType()
{
	if (auto& type = m_Types["str"])
		return std::dynamic_pointer_cast<StrType>(type);
	auto type = std::make_shared<StrType>();
	m_Types["str"] = type;
	return type;
}

csaw::codegen::EmptyTypePtr csaw::codegen::Context::GetEmptyType()
{
	if (auto& type = m_Types[""])
		return std::dynamic_pointer_cast<EmptyType>(type);
	auto type = std::make_shared<EmptyType>();
	m_Types[""] = type;
	return type;
}

csaw::codegen::ThingTypePtr csaw::codegen::Context::GetThingType(const std::string& name)
{
	if (auto& type = m_Types[name])
		return std::dynamic_pointer_cast<ThingType>(type);
	return nullptr;
}

csaw::codegen::ThingTypePtr csaw::codegen::Context::CreateThingType(const std::string& name, const std::map<std::string, TypePtr>& elements)
{
	auto type = std::make_shared<ThingType>(name, elements);
	m_Types[name] = type;
	return type;
}

csaw::codegen::FunctionTypePtr csaw::codegen::Context::GetFunctionType(TypePtr result, const std::vector<TypePtr>& argtypes, bool isvararg)
{
	auto ftype = std::make_shared<FunctionType>(result, argtypes, isvararg);
	auto name = ftype->GetName();
	if (auto& type = m_Types[name])
		return std::dynamic_pointer_cast<FunctionType>(type);
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
		args.push_back(std::make_shared<Arg>("arg" + std::to_string(i), type->ArgTypes[i]));

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

std::string& csaw::codegen::Context::Filepath()
{
	return m_Filepath.back();
}

const std::string& csaw::codegen::Context::Filepath() const
{
	return m_Filepath.back();
}

const std::map<std::string, csaw::codegen::TypePtr>& csaw::codegen::Context::Types() const
{
	return m_Types;
}

const std::map<csaw::codegen::TypePtr, std::map<std::string, std::map<csaw::codegen::FunctionTypePtr, csaw::codegen::FunctionPtr>>>& csaw::codegen::Context::Functions() const
{
	return m_Functions;
}

const std::vector<std::string>& csaw::codegen::Context::Filepaths() const
{
	return m_Filepath;
}

void csaw::codegen::Context::PushFilepath(const std::string& filepath)
{
	m_Filepath.push_back(filepath);
}

void csaw::codegen::Context::PopFilepath()
{
	m_Filepath.pop_back();
}

void csaw::codegen::Context::ClearVariables()
{
	m_Variables.clear();
}

void csaw::codegen::Context::SetVariable(const std::string& name, TypePtr type)
{
	m_Variables[name] = type;
}
