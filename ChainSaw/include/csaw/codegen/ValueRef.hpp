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

        llvm::Value* GetValue() const;
        llvm::Type* GetType() const;
        llvm::Type* GetBaseType() const;
        TypePtr GetRawType() const;
        TypePtr GetRawBaseType() const;

        ValueRef GetReference() const;
        bool IsRValue() const;

        ValueRef Load() const;
        const ValueRef& Store(const ValueRef& value) const;

        bool Invalid() const;

    private:
        ValueRef(Builder* builder, bool rvalue, llvm::Value* value, const TypePtr& rawType);

        void Check() const;

        Builder* m_Builder;
        bool m_IsRValue;
        llvm::Value* m_Value;
        TypePtr m_RawType;
    };
}
