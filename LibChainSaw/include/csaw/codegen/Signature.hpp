#pragma once

#include <string>
#include <vector>
#include <csaw/Type.hpp>
#include <llvm/IR/Function.h>

namespace csaw
{
    struct Signature
    {
        static Signature Demangle(const llvm::Function& function);

        std::string Mangle() const;
        bool IsConstructor() const;

        std::string Name;
        TypePtr Parent;
        TypePtr Result;
        std::vector<TypePtr> Args;
        bool IsVarargs = false;
        bool IsC = false;
    };
}
