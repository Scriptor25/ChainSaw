#pragma once

#include <csaw/Type.hpp>
#include <llvm/IR/Value.h>

namespace csaw
{
    struct FunctionRef
    {
        FunctionRef();
        FunctionRef(llvm::Function* function, const std::string& name, bool constructor, const TypePtr& callee, const std::vector<TypePtr>& args, bool vararg, const TypePtr& result);

        llvm::Function* Function;

        std::string Name;
        bool IsConstructor;
        TypePtr Callee;
        std::vector<TypePtr> Args;
        bool IsVarArg;
        TypePtr Result;
    };
}
