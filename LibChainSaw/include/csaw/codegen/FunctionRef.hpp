#pragma once

#include <csaw/Type.hpp>
#include <llvm/IR/Function.h>

namespace csaw
{
    struct FunctionRef
    {
        FunctionRef(llvm::Function* function, const std::string& name, const TypePtr& callee, const TypePtr& result, const std::vector<TypePtr>& args, bool isConstructor, bool isVarArg);

        llvm::Function* Function;

        std::string Name;
        TypePtr Callee;
        TypePtr Result;
        std::vector<TypePtr> Args;
        bool IsConstructor;
        bool IsVarArg;
    };
}
