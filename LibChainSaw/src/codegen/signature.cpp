#include <sstream>
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

static csaw::TypePtr from_llvm(const llvm::Type* type)
{
    if (type->isPointerTy()) return csaw::PointerType::Get(csaw::Type::GetVoid());
    if (type->isVoidTy()) return csaw::Type::GetVoid();
    if (type->isIntegerTy(1)) return csaw::Type::GetInt1();
    if (type->isIntegerTy(8)) return csaw::Type::GetInt8();
    if (type->isIntegerTy(16)) return csaw::Type::GetInt16();
    if (type->isIntegerTy(32)) return csaw::Type::GetInt32();
    if (type->isIntegerTy(64)) return csaw::Type::GetInt64();
    if (type->isIntegerTy(128)) return csaw::Type::GetInt128();
    if (type->isHalfTy()) return csaw::Type::GetFlt16();
    if (type->isFloatTy()) return csaw::Type::GetFlt32();
    if (type->isDoubleTy()) return csaw::Type::GetFlt64();
    return nullptr;
}

csaw::Signature csaw::Signature::Demangle(const llvm::Function& function)
{
    auto v = split(function.getName().str(), ',');

    Signature s;
    s.Name = pop_front(v);
    if (v.empty())
    {
        s.IsC = true;
        s.Result = from_llvm(function.getFunctionType()->getReturnType());
        for (const auto arg : function.getFunctionType()->params())
            s.Args.push_back(from_llvm(arg));
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

std::string csaw::Signature::Mangle() const
{
    // <name>(,$,<parent>),<argc>(,[<arg1>,<arg2> ...])(,?)(,<result>)
    std::string name;

    name += Name;
    if (IsC) return name;
    if (Parent) name += ",$," + Parent->Name;
    name += ',' + std::to_string(Args.size());
    for (const auto& arg : Args)
        name += ',' + arg->Name;
    if (IsVarargs) name += ",?";
    if (Result) name += ',' + Result->Name;

    return name;
}

bool csaw::Signature::IsConstructor() const
{
    return !Parent && !Result;
}
