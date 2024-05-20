#pragma once

#include <csaw/Type.hpp>
#include <csaw/codegen/Def.hpp>
#include <llvm/IR/Value.h>

namespace csaw
{
    class Value
    {
    public:
        virtual ~Value() = default;

        virtual TypePtr GetType() const = 0;
        virtual llvm::Value* GetValue() const = 0;

        virtual bool IsLValue() const = 0;
        virtual RValuePtr GetRValue() const = 0;

        virtual llvm::Value* GetBoolValue(const Builder* builder) const;
    };

    class LValue : public Value
    {
    public:
        static Expect<LValuePtr> Allocate(Builder* builder, const TypePtr& type);
        static Expect<LValuePtr> AllocateAndStore(Builder* builder, const TypePtr& type, llvm::Value* value);
        static LValuePtr Direct(Builder* builder, const TypePtr& type, llvm::Value* pointer);

        void StoreValue(llvm::Value* value) const;
        RValuePtr GetReference() const;
        Expect<LValuePtr> Dereference() const;
        llvm::Value* GetPointer() const;

        TypePtr GetType() const override;
        llvm::Value* GetValue() const override;

        bool IsLValue() const override;
        RValuePtr GetRValue() const override;

    private:
        LValue(Builder* builder, const TypePtr& type, llvm::Value* pointer);

        Builder* m_Builder = nullptr;
        TypePtr m_Type; // Type of value stored in pointer
        llvm::Value* m_Pointer = nullptr;
    };

    class RValue : public Value
    {
    public:
        static RValuePtr Create(const TypePtr& type, llvm::Value* value);

        Expect<LValuePtr> Dereference(Builder* builder) const;

        TypePtr GetType() const override;
        llvm::Value* GetValue() const override;

        bool IsLValue() const override;
        RValuePtr GetRValue() const override;

    private:
        RValue(const TypePtr& type, llvm::Value* value);

        TypePtr m_Type; // Type of value
        llvm::Value* m_Value;
    };
}
