#pragma once

#include <csaw/Type.hpp>
#include <csaw/codegen/Def.hpp>
#include <llvm/IR/Value.h>

namespace csaw
{
    enum ValueRefMode
    {
        ValueRefMode_Invalid,
        ValueRefMode_Constant,
        ValueRefMode_Pointer,
        ValueRefMode_AllocateValue,
    };

    struct ValueRef
    {
        ValueRef();
        ValueRef(Builder& builder, ValueRefMode mode, llvm::Value* value, const TypePtr& rawType);

        [[nodiscard]] TypePtr RawType() const;
        [[nodiscard]] llvm::Type* Type() const;
        [[nodiscard]] ValueRefMode Mode() const;

        [[nodiscard]] llvm::Value* Load(Builder& builder) const;
        llvm::Value* Store(Builder& builder, llvm::Value* value) const;

    private:
        ValueRefMode m_Mode;
        llvm::Value* m_Pointer;
        TypePtr m_RawType;
        llvm::Type* m_Type;
    };
}
