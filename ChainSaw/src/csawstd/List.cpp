#include <csawstd/List.h>
#include <runtime/Const.h>

using namespace csaw::runtime;

void csawstd::InitList()
{
	Environment::CreateThing("list", "", { {"ptr", "str"} });
	Environment::CreateFunction(true, "list", "list", {}, false, "", list);
	Environment::CreateFunction(false, "add", "", { "" }, false, "list", add);
	Environment::CreateFunction(false, "size", "num", {}, false, "list", size);
	Environment::CreateFunction(false, "get", "", { "num" }, false, "list", get);
}

csawstd::List* csawstd::List::FromStr(const std::string& str)
{
	return (List*)std::stoll(str);
}

std::string csawstd::List::ToStr()
{
	return std::to_string((intptr_t)this);
}

void csawstd::List::Add(ValuePtr value)
{
	Values.push_back(value);
}

ValuePtr csawstd::list(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	auto ptr = new List();
	return ConstThing::Ptr("list", { {"ptr", ConstStr::Ptr(ptr->ToStr())} });
}

ValuePtr csawstd::add(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	auto ptr = List::FromStr(callee->GetElement("ptr")->GetStr());
	ptr->Add(args[0]);
	return ValuePtr();
}

ValuePtr csawstd::size(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	auto ptr = List::FromStr(callee->GetElement("ptr")->GetStr());
	return ConstNum::Ptr(double(ptr->Values.size()));
}

ValuePtr csawstd::get(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	auto ptr = List::FromStr(callee->GetElement("ptr")->GetStr());
	size_t i = size_t(args[0]->GetNum());
	return ptr->Values[i];
}
