#pragma once

#include <map>
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
        static std::string GetFunctionName(const std::string& name, bool constructor, const TypePtr& callee, const std::vector<TypePtr>& args, bool vararg);
        static void GetFunctionAttributes(const std::string& functionName, std::string& name, bool& constructor, TypePtr& callee, std::vector<TypePtr>& args, bool& vararg);

    public:
        explicit Builder(const std::string& moduleName);

        llvm::Function* GetFunction(const std::string& name, const TypePtr& callee, const std::vector<TypePtr>& args);
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
        llvm::Value* Gen(const CastExpression& expression);
        llvm::Value* Gen(const CharExpression& expression);
        llvm::Value* Gen(const FloatExpression& expression);
        llvm::Value* Gen(const IdentifierExpression& expression);
        llvm::Value* Gen(const IndexExpression& expression);
        llvm::Value* Gen(const IntExpression& expression);
        llvm::Value* Gen(const MemberExpression& expression);
        llvm::Value* Gen(const SelectExpression& expression);
        llvm::Value* Gen(const StringExpression& expression);
        llvm::Value* Gen(const UnaryExpression& expression);
        llvm::Value* Gen(const VarArgExpression& expression);

        llvm::Type* Gen(const TypePtr& ptr);

        llvm::Value* GenCmpEQ(llvm::Value* left, llvm::Value* right);
        llvm::Value* GenCmpNE(llvm::Value* left, llvm::Value* right);
        llvm::Value* GenCmpLE(llvm::Value* left, llvm::Value* right);
        llvm::Value* GenCmpGE(llvm::Value* left, llvm::Value* right);
        llvm::Value* GenCmpLT(llvm::Value* left, llvm::Value* right);
        llvm::Value* GenCmpGT(llvm::Value* left, llvm::Value* right);
        llvm::Value* GenAdd(llvm::Value* left, llvm::Value* right) const;
        llvm::Value* GenSub(llvm::Value* left, llvm::Value* right) const;
        llvm::Value* GenMul(llvm::Value* left, llvm::Value* right) const;
        llvm::Value* GenDiv(llvm::Value* left, llvm::Value* right);
        llvm::Value* GenRem(llvm::Value* left, llvm::Value* right);

        [[nodiscard]]
        bool IsGlobal() const;

    private:
        std::unique_ptr<llvm::LLVMContext> m_Context;
        std::unique_ptr<llvm::IRBuilder<>> m_Builder;
        std::unique_ptr<llvm::Module> m_Module;

        std::map<std::string, llvm::Value*> m_Values;
    };
}
