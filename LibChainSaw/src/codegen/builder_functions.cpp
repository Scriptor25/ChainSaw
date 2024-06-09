#include <csaw/Builder.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

llvm::Function* csaw::Builder::FindFunctionBySignature(const Signature& sig)
{
    for (const auto& [function, signature] : m_Signatures)
        if (sig == signature)
            return function;
    return nullptr;
}

std::pair<llvm::Function*, csaw::Signature> csaw::Builder::FindBestFunctionBySignature(const Signature& sig)
{
    int highscore = 0;
    std::pair<llvm::Function*, Signature> best;

    for (const auto& [function, signature] : m_Signatures)
    {
        if (signature == sig) return {function, signature};

        if (signature.Name != sig.Name) continue;
        if (signature.Parent != sig.Parent) continue;
        if (signature.Args.size() > sig.Args.size() || (!signature.IsVarargs && signature.Args.size() != sig.Args.size())) continue;

        size_t i = 0;
        for (; i < signature.Args.size(); ++i)
            if (!signature.Args[i]->ParentOf(sig.Args[i]))
                break;
        if (i < signature.Args.size()) continue;

        int score = 0;
        for (i = 0; i < signature.Args.size(); ++i)
        {
            if (signature.Args[i] == sig.Args[i])
                score += 100;
            else if (signature.Args[i]->ParentOf(sig.Args[i]))
                score += 50;
        }

        score += static_cast<int>(sig.Args.size() - (sig.Args.size() - signature.Args.size()) - 1) * 10;
        if (signature.IsVarargs == sig.IsVarargs) score += 200;
        if (signature.Result == sig.Result) score += 200;

        if (score >= highscore)
        {
            highscore = score;
            best = {function, signature};
        }
    }

    return best;
}

csaw::Expect<csaw::RValuePtr> csaw::Builder::CreateCall(const ValuePtr& callee, const LValuePtr& parent, const std::vector<ValuePtr>& args)
{
    if (!callee->GetType()->IsFunctionPointer())
        return Expect<RValuePtr>("Callee is not a function pointer");

    const auto fnty = callee->GetType()->AsPointer()->Base->AsFunction();
    const auto type = Gen(fnty);
    if (!type)
        return Expect<RValuePtr>("Failed to generate type " + fnty->Name + ": " + type.Msg());

    std::vector<llvm::Value*> llvm_args;
    if (parent) llvm_args.push_back(parent->GetPointer());
    size_t i = 0;
    for (; i < fnty->Args.size(); ++i)
    {
        const auto cast = Cast(args[i], fnty->Args[i]);
        if (!cast)
            return Expect<RValuePtr>("Failed to cast: " + cast.Msg());
        llvm_args.push_back(cast.Get()->GetValue());
    }
    for (; i < args.size(); ++i)
        llvm_args.push_back(args[i]->GetValue());

    const auto value = GetBuilder().CreateCall(llvm::cast<llvm::FunctionType>(type.Get()), callee->GetValue(), llvm_args);
    return RValue::Create(this, fnty->Result, value);
}

csaw::Expect<csaw::LValuePtr> csaw::Builder::CreateCtorCall(const ValuePtr& callee, const TypePtr& ctortype, const std::vector<ValuePtr>& args)
{
    if (!callee->GetType()->IsFunctionPointer())
        return Expect<LValuePtr>("Callee is not a function pointer");

    const auto fnty = callee->GetType()->AsPointer()->Base->AsFunction();

    const auto result = Gen(ctortype);
    if (!result)
        return Expect<LValuePtr>("Failed to generate type " + ctortype->Name + ": " + result.Msg());

    std::vector<llvm::Type*> arg_types;
    arg_types.push_back(GetBuilder().getPtrTy());
    for (const auto& arg : fnty->Args)
    {
        const auto gen = Gen(arg);
        if (!gen)
            return Expect<LValuePtr>("Failed to generate type " + arg->Name + ": " + gen.Msg());
        arg_types.push_back(gen.Get());
    }

    llvm::FunctionType* type = llvm::FunctionType::get(result.Get(), arg_types, fnty->IsVararg);

    const auto alloc = LValue::Allocate(this, ctortype);
    if (!alloc)
        return Expect<LValuePtr>("Failed to allocate: " + alloc.Msg());

    const auto& value = alloc.Get();

    std::vector<llvm::Value*> llvm_args;
    llvm_args.push_back(value->GetPointer());
    size_t i = 0;
    for (; i < fnty->Args.size(); ++i)
    {
        const auto cast = Cast(args[i], fnty->Args[i]);
        if (!cast)
            return Expect<LValuePtr>("Failed to cast: " + cast.Msg());
        llvm_args.push_back(cast.Get()->GetValue());
    }
    for (; i < args.size(); ++i)
        llvm_args.push_back(args[i]->GetValue());

    GetBuilder().CreateCall(type, callee->GetValue(), llvm_args);
    return value;
}

csaw::Expect<csaw::RValuePtr> csaw::Builder::FindBestAndCall(const std::string& name, const LValuePtr& parent, const std::vector<ValuePtr>& args)
{
    std::vector<TypePtr> arg_types;
    for (const auto& arg : args)
        arg_types.push_back(arg->GetType());

    const auto [function, signature] = FindBestFunction(name, parent ? parent->GetType() : nullptr, arg_types);
    if (!function)
        return Expect<RValuePtr>("");

    const ValuePtr callee = RValue::Create(this, PointerType::Get(signature.GetFunctionType()), function);

    auto value = CreateCall(callee, parent, args);
    if (!value)
        return Expect<RValuePtr>("Call failed: " + value.Msg());

    return value;
}

csaw::Expect<std::shared_ptr<csaw::LValue>> csaw::Builder::FindBestCtorAndCall(const std::string& name, const std::vector<ValuePtr>& args)
{
    std::vector<TypePtr> arg_types;
    for (const auto& arg : args)
        arg_types.push_back(arg->GetType());

    const auto [function, signature] = FindBestFunction(name, nullptr, arg_types, true);
    if (!function)
        return Expect<LValuePtr>("");

    const ValuePtr callee = RValue::Create(this, PointerType::Get(signature.GetFunctionType()), function);

    auto value = CreateCtorCall(callee, Type::Get(signature.Name), args);
    if (!value)
        return Expect<LValuePtr>("Call failed: " + value.Msg());

    return value;
}

std::map<llvm::Function*, csaw::Signature> csaw::Builder::FindFunctions(const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args, const bool is_ctor) const
{
    std::map<llvm::Function*, Signature> functions;
    for (const auto& [function, signature] : m_Signatures)
    {
        if (signature.IsConstructor() != is_ctor) continue;
        if (signature.Name != name) continue;
        if (signature.Parent != parent) continue;
        if (signature.Args.size() > args.size() || (!signature.IsVarargs && signature.Args.size() != args.size())) continue;
        size_t i = 0;
        for (; i < signature.Args.size(); ++i)
            if (!signature.Args[i]->ParentOf(args[i]))
                break;
        if (i < signature.Args.size()) continue;
        functions[function] = signature;
    }

    return functions;
}

std::pair<llvm::Function*, csaw::Signature> csaw::Builder::FindBestFunction(const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args, const bool is_ctor) const
{
    const auto functions = FindFunctions(name, parent, args, is_ctor);

    int highscore = 0;
    std::pair<llvm::Function*, Signature> best;

    for (const auto& [function, signature] : functions)
    {
        if (const int score = RankFunction(signature, name, parent, args); score >= highscore)
        {
            highscore = score;
            best = {function, signature};
        }
    }

    return best;
}

int csaw::Builder::RankFunction(const Signature& signature, const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    int score = 0;

    if (signature.Name == name) score += 100;
    if (signature.Parent == parent) score += 100;

    for (size_t i = 0; i < signature.Args.size(); ++i)
    {
        if (signature.Args[i] == args[i])
            score += 100;
        else if (signature.Args[i]->ParentOf(args[i]))
            score += 50;
    }

    score += static_cast<int>(args.size() - (args.size() - signature.Args.size()) - 1) * 10;

    return score;
}
