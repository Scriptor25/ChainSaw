#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/FunctionRef.hpp>

csaw::FunctionRef::FunctionRef(llvm::Function* function, const std::string& name, const TypePtr& callee, const TypePtr& result, const std::vector<TypePtr>& args, const bool isConstructor, const bool isVarArg)
    : Function(function), Name(name), Callee(callee), Result(result), Args(args), IsConstructor(isConstructor), IsVarArg(isVarArg)
{
}

const csaw::FunctionRef* csaw::Builder::GetFunction(const std::string& name, const TypePtr& callee, const std::vector<TypePtr>& args)
{
    for (const auto& ref : m_Functions[name])
    {
        if (name != ref.Name) continue;
        if (callee != ref.Callee) continue;
        if (args.size() < ref.Args.size() || (args.size() > ref.Args.size() && !ref.IsVarArg)) continue;

        size_t i = 0;
        for (; i < ref.Args.size(); ++i)
            if (ref.Args[i] != args[i]) break;
        if (i < ref.Args.size()) continue;

        return &ref;
    }

    return nullptr;
}

csaw::FunctionRef& csaw::Builder::GetOrCreateFunction(const std::string& name, const TypePtr& callee, const TypePtr& result, const std::vector<TypePtr>& args, const bool isConstructor, const bool isVarArg)
{
    for (auto& ref : m_Functions[name])
    {
        if (name != ref.Name) continue;
        if (isConstructor != ref.IsConstructor) continue;
        if (callee != ref.Callee) continue;
        if (args.size() != ref.Args.size()) continue;
        if (isVarArg != ref.IsVarArg) continue;
        if (result != ref.Result) continue;

        size_t i = 0;
        for (; i < ref.Args.size(); ++i)
            if (ref.Args[i] != args[i]) break;
        if (i < ref.Args.size()) continue;

        return ref;
    }

    return m_Functions[name].emplace_back(nullptr, name, callee, result, args, isConstructor, isVarArg);
}

std::string csaw::Builder::FunctionSignatureString(const TypePtr& callee, const std::vector<TypePtr>& args)
{
    std::string signature;
    if (callee) signature += callee->Name;
    else signature += "<none>";
    signature += '(';
    for (size_t i = 0; i < args.size(); ++i)
    {
        if (i > 0) signature += ", ";
        signature += args[i]->Name;
    }
    signature += ')';
    return signature;
}
