#include <csaw/codegen/Builder.hpp>

std::string csaw::Builder::GetFunctionName(const std::string& name, const bool constructor, const TypePtr& callee, const std::vector<TypePtr>& args, const bool vararg)
{
    std::string function_name;

    if (constructor)
        function_name += '$';

    function_name += name;

    if (callee)
        function_name += ':' + callee->Name;

    if (!args.empty() || vararg)
    {
        function_name += '(';
        size_t i = 0;
        for (; i < args.size(); ++i)
        {
            if (i > 0) function_name += ',';
            function_name += args[i]->Name;
        }
        if (vararg)
        {
            if (i > 0) function_name += ',';
            function_name += '?';
        }
        function_name += ')';
    }

    return function_name;
}

void csaw::Builder::GetFunctionAttributes(const std::string& functionName, std::string& name, bool& constructor, TypePtr& callee, std::vector<TypePtr>& args, bool& vararg)
{
    name.clear();
    constructor = false;
    callee = nullptr;
    args.clear();
    vararg = false;

    auto copy = functionName;

    if (copy[0] == '$')
    {
        constructor = true;
        copy = copy.substr(1);
    }

    const auto callee_index = copy.find(':');
    const auto args_index = copy.find('(');

    if (callee_index == std::string::npos && args_index == std::string::npos)
    {
        name = copy;
        return;
    }

    // name:callee(...)
    if (callee_index != std::string::npos)
    {
        name = copy.substr(0, callee_index);
        if (args_index == std::string::npos)
        {
            copy = copy.substr(callee_index + 1);
            callee = Type::Get(copy);
            return;
        }

        callee = Type::Get(copy.substr(callee_index + 1, args_index - callee_index - 1));
        copy = copy.substr(args_index + 1);
    }
    else
    {
        name = copy.substr(0, args_index);
        copy = copy.substr(args_index + 1);
    }

    size_t arg_end = copy.find(',');
    while (arg_end != std::string::npos)
    {
        args.push_back(Type::Get(copy.substr(0, arg_end)));
        copy = copy.substr(arg_end + 1);
        arg_end = copy.find(',');
    }
    if (copy[0] == '?') vararg = true;
}

csaw::Builder::Builder(const std::string& moduleName)
{
    m_Context = std::make_unique<llvm::LLVMContext>();
    m_Builder = std::make_unique<llvm::IRBuilder<>>(*m_Context);
    m_Module = std::make_unique<llvm::Module>(moduleName, *m_Context);
}

llvm::Function* csaw::Builder::GetFunction(const std::string& name, const TypePtr& callee, const std::vector<TypePtr>& args)
{
    std::string _name;
    bool _constructor;
    TypePtr _callee;
    std::vector<TypePtr> _args;
    bool _vararg;

    auto& functions = m_Module->getFunctionList();
    for (auto& function : functions)
    {
        GetFunctionAttributes(function.getName().str(), _name, _constructor, _callee, _args, _vararg);
        if (name != _name) continue;
        if (_callee != callee) continue;
        if (args.size() < _args.size() || (args.size() > _args.size() && !_vararg)) continue;

        size_t i = 0;
        for (; i < _args.size(); ++i)
            if (_args[i] != args[i]) break;
        if (i < _args.size()) continue;

        return &function;
    }
    throw std::runtime_error("not yet implemented");
}

bool csaw::Builder::IsGlobal() const
{
    return !m_Builder->GetInsertBlock();
}
