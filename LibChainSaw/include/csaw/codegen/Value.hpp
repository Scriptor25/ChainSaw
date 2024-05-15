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
        virtual bool IsRValue() const = 0;

        virtual RValuePtr GetRValue() = 0;
    };

    class LValue : public Value
    {
    public:
        static LValuePtr Allocate(Builder* builder, const TypePtr& type);
        static LValuePtr AllocateAndStore(Builder* builder, const TypePtr& type, llvm::Value* value);
        static LValuePtr Direct(Builder* builder, const TypePtr& type, llvm::Value* pointer);

        void StoreValue(llvm::Value* value) const;
        RValuePtr GetReference() const;
        LValuePtr Dereference() const;
        llvm::Value* GetPointer() const;

        TypePtr GetType() const override;
        llvm::Value* GetValue() const override;

        bool IsLValue() const override;
        bool IsRValue() const override;

        RValuePtr GetRValue() override;

    private:
        LValue(Builder* builder, const TypePtr& type, llvm::Value* pointer);

        Builder* m_Builder;
        TypePtr m_Type; // Type of value stored in pointer
        llvm::Value* m_Pointer;
    };

    class RValue
            : public Value,
              public std::enable_shared_from_this<RValue>
    {
    public:
        static RValuePtr Direct(const TypePtr& type, llvm::Value* value);

        LValuePtr Dereference(Builder* builder) const;

        TypePtr GetType() const override;
        llvm::Value* GetValue() const override;

        bool IsLValue() const override;
        bool IsRValue() const override;

        RValuePtr GetRValue() override;

    private:
        RValue(const TypePtr& type, llvm::Value* value);

        TypePtr m_Type; // Type of value
        llvm::Value* m_Value;
    };
}
