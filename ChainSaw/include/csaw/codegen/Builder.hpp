#pragma once

#include <memory>
#include <csaw/Type.hpp>
#include <csaw/lang/Def.hpp>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace csaw
{
    class Builder
    {
    public:
        explicit Builder(const std::string& moduleName);

        void Gen(const StatementPtr& ptr);

    private:
        void Gen(const ScopeStatement& statement);
        void Gen(const ForStatement& statement);
        void Gen(const FunctionStatement& statement);
        void Gen(const IfStatement& statement);
        void Gen(const RetStatement& statement);
        void Gen(const DefStatement& statement);
        void Gen(const VariableStatement& statement);
        void Gen(const WhileStatement& statement);

        llvm::Value* Gen(const ExpressionPtr& ptr);
        llvm::Value* Gen(const BinaryExpression& expression);
        llvm::Value* Gen(const CallExpression& expression);
        llvm::Value* Gen(const CharExpression& expression);
        llvm::Value* Gen(const IdentifierExpression& expression);
        llvm::Value* Gen(const IndexExpression& expression);
        llvm::Value* Gen(const MemberExpression& expression);
        llvm::Value* Gen(const NumberExpression& expression);
        llvm::Value* Gen(const SelectExpression& expression);
        llvm::Value* Gen(const StringExpression& expression);
        llvm::Value* Gen(const UnaryExpression& expression);
        llvm::Value* Gen(const VarArgExpression& expression);

        llvm::Type* Gen(const TypePtr& ptr);

        [[nodiscard]] bool IsGlobal() const;

    private:
        std::unique_ptr<llvm::LLVMContext> m_Context;
        std::unique_ptr<llvm::IRBuilder<>> m_Builder;
        std::unique_ptr<llvm::Module> m_Module;
    };
}
