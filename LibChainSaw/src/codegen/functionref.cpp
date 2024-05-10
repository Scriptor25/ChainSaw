#include <csaw/codegen/FunctionRef.hpp>

csaw::FunctionRef::FunctionRef(llvm::Function* function, const std::string& name, const TypePtr& callee, const TypePtr& result, const std::vector<TypePtr>& args, const bool isConstructor, const bool isVarArg)
    : Function(function), Name(name), Callee(callee), Result(result), Args(args), IsConstructor(isConstructor), IsVarArg(isVarArg)
{
}
