#pragma once

#include <csaw/Type.hpp>
#include <csaw/codegen/Def.hpp>
#include <llvm/IR/Value.h>

namespace csaw
{
    class ValueRef
    {
    public:
        static ValueRef Allocate(Builder* builder, llvm::Value* value, const TypePtr& rawBaseType);
        static ValueRef Constant(Builder* builder, llvm::Value* value, const TypePtr& rawType);
        static ValueRef Pointer(Builder* builder, llvm::Value* value, const TypePtr& rawBaseType);

        ValueRef();

        [[nodiscard]] llvm::Value* GetValue() const;
        [[nodiscard]] llvm::Type* GetType() const;
        [[nodiscard]] llvm::Type* GetBaseType() const;
        [[nodiscard]] llvm::Type* GetBaseTypeBase() const;
        [[nodiscard]] TypePtr GetRawType() const;
        [[nodiscard]] TypePtr GetRawBaseType() const;
        [[nodiscard]] TypePtr GetRawBaseTypeBase() const;

        [[nodiscard]] ValueRef GetReference() const;
        [[nodiscard]] bool IsRValue() const;

        [[nodiscard]] ValueRef Load() const;
        [[nodiscard]] const ValueRef& Store(const ValueRef& value) const;

        [[nodiscard]] bool Invalid() const;

    private:
        ValueRef(Builder* builder, bool rvalue, llvm::Value* value, const TypePtr& rawType);

        void Check() const;

        Builder* m_Builder;
        bool m_IsRValue;
        llvm::Value* m_Value;
        TypePtr m_RawType;
    };
}
