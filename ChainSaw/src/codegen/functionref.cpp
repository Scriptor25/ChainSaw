#include <csaw/codegen/FunctionRef.hpp>

csaw::FunctionRef::FunctionRef()
{
    Function = nullptr;
    IsConstructor = false;
    IsVarArg = false;
}

csaw::FunctionRef::FunctionRef(llvm::Function* function, const std::string& name, const bool constructor, const TypePtr& callee, const std::vector<TypePtr>& args, const bool vararg, const TypePtr& result)
    : Function(function), Name(name), IsConstructor(constructor), Callee(callee), Args(args), IsVarArg(vararg), Result(result)
{
}
