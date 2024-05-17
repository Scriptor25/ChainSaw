#include <sstream>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Signature.hpp>

static std::vector<std::string> split(const std::string& str, const char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while (getline(ss, item, delim))
        result.push_back(item);

    return result;
}

static std::string pop_front(std::vector<std::string>& vec)
{
    auto str = vec[0];
    vec.erase(vec.begin());
    return str;
}

csaw::Signature csaw::Signature::Demangle(const llvm::Function& function)
{
    auto v = split(function.getName().str(), '&');

    Signature s;
    s.Name = pop_front(v);
    if (v.empty())
    {
        s.IsC = true;
        s.Result = Builder::FromLLVM(function.getFunctionType()->getReturnType());
        for (const auto arg : function.getFunctionType()->params())
            s.Args.push_back(Builder::FromLLVM(arg));
        s.IsVarargs = function.getFunctionType()->isVarArg();
        return s;
    }

    if (v[0][0] == '$')
    {
        pop_front(v);
        s.Parent = Type::Get(pop_front(v));
    }

    s.Args.resize(std::stoull(pop_front(v)));
    for (size_t i = 0; i < s.Args.size(); ++i)
        s.Args[i] = Type::Get(pop_front(v));
    if (!v.empty() && v[0][0] == '?')
    {
        pop_front(v);
        s.IsVarargs = true;
    }
    if (!v.empty())
        s.Result = Type::Get(pop_front(v));

    return s;
}

std::string csaw::Signature::Mangle(const bool ignore_c) const
{
    // <name>(&$&<parent>)&<argc>(&[<arg1>&<arg2> ...])(&?)(&<result>)
    std::string name;

    name += Name;
    if (!ignore_c && IsC) return name;
    if (Parent) name += "&$&" + Parent->Name;
    name += '&' + std::to_string(Args.size());
    for (const auto& arg : Args)
        name += '&' + arg->Name;
    if (IsVarargs) name += "&?";
    if (Result) name += '&' + Result->Name;

    return name;
}

bool csaw::Signature::IsConstructor() const
{
    return !Parent && !Result;
}
