#pragma once

#include <csaw/Def.hpp>
#include <llvm/IR/Value.h>

namespace csaw
{
    class Value
    {
    public:
        virtual ~Value() = default;

        [[nodiscard]] virtual TypePtr GetType() const = 0;
        [[nodiscard]] virtual llvm::Value* GetValue() const = 0;

        [[nodiscard]] virtual bool IsLValue() const = 0;
        [[nodiscard]] virtual RValuePtr GetRValue() const = 0;

        [[nodiscard]] virtual Expect<LValuePtr> Dereference() const = 0;

        [[nodiscard]] virtual llvm::Value* GetBoolValue() const = 0;
    };

    class LValue : public Value
    {
    public:
        static Expect<LValuePtr> Allocate(Builder* builder, const TypePtr& type);
        static Expect<LValuePtr> AllocateAndStore(Builder* builder, const TypePtr& type, const ValuePtr& value);
        static LValuePtr Direct(Builder* builder, const TypePtr& type, llvm::Value* pointer);

        [[nodiscard]] csaw::Expect<csaw::ValuePtr> StoreValue(const ValuePtr& value) const;
        [[nodiscard]] RValuePtr GetReference() const;
        [[nodiscard]] llvm::Value* GetPointer() const;

        [[nodiscard]] TypePtr GetType() const override;
        [[nodiscard]] llvm::Value* GetValue() const override;

        [[nodiscard]] bool IsLValue() const override;
        [[nodiscard]] RValuePtr GetRValue() const override;

        [[nodiscard]] Expect<LValuePtr> Dereference() const override;

        [[nodiscard]] llvm::Value* GetBoolValue() const override;

    private:
        LValue(Builder* builder, const TypePtr& type, llvm::Value* pointer);

        Builder* m_Builder;
        TypePtr m_Type; // Type of value stored in pointer
        llvm::Value* m_Pointer;
    };

    class RValue : public Value
    {
    public:
        static RValuePtr Create(Builder* builder, const TypePtr& type, llvm::Value* value);

        [[nodiscard]] TypePtr GetType() const override;
        [[nodiscard]] llvm::Value* GetValue() const override;

        [[nodiscard]] bool IsLValue() const override;
        [[nodiscard]] RValuePtr GetRValue() const override;

        [[nodiscard]] Expect<LValuePtr> Dereference() const override;

        [[nodiscard]] llvm::Value* GetBoolValue() const override;

    private:
        RValue(Builder* builder, const TypePtr& type, llvm::Value* value);

        Builder* m_Builder;
        TypePtr m_Type; // Type of value
        llvm::Value* m_Value;
    };
}
