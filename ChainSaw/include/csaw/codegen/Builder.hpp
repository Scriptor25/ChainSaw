#pragma once

#include <map>
#include <memory>
#include <vector>
#include <csaw/Type.hpp>
#include <csaw/codegen/CSawJIT.hpp>
#include <csaw/codegen/FunctionRef.hpp>
#include <csaw/codegen/ValueRef.hpp>
#include <csaw/lang/Def.hpp>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/StandardInstrumentations.h>

namespace csaw
{
    class Builder
    {
    public:
        explicit Builder(const std::string& moduleName);

        llvm::LLVMContext& GetContext();
        llvm::IRBuilder<>& GetBuilder();
        llvm::Module& GetModule();

        int Main(int argc, char** argv);

        const FunctionRef& GetFunction(const std::string& name, const TypePtr& callee, const std::vector<TypePtr>& args);
        FunctionRef& GetOrCreateFunction(const std::string& name, bool constructor, const TypePtr& callee, const std::vector<TypePtr>& args, bool vararg, const TypePtr& result);

        bool IsGlobal() const;

        void Gen(const StatementPtr& ptr);
        llvm::Type* Gen(const TypePtr& ptr);

    private:
        void Gen(const ScopeStatement& statement);
        void Gen(const ForStatement& statement);
        void Gen(const FunctionStatement& statement);
        void Gen(const IfStatement& statement);
        void Gen(const RetStatement& statement);
        void Gen(const DefStatement& statement);
        void Gen(const VariableStatement& statement);
        void Gen(const WhileStatement& statement);

        ValueRef Gen(const ExpressionPtr& ptr);
        ValueRef Gen(const BinaryExpression& expression);
        ValueRef Gen(const CallExpression& expression);
        ValueRef Gen(const CastExpression& expression);
        ValueRef Gen(const CharExpression& expression);
        ValueRef Gen(const FloatExpression& expression);
        ValueRef Gen(const IdentifierExpression& expression);
        ValueRef Gen(const IndexExpression& expression);
        ValueRef Gen(const IntExpression& expression);
        ValueRef Gen(const MemberExpression& expression);
        ValueRef Gen(const ReferenceExpression& expression);
        ValueRef Gen(const SelectExpression& expression);
        ValueRef Gen(const StringExpression& expression);
        ValueRef Gen(const UnaryExpression& expression);
        ValueRef Gen(const VarArgExpression& expression);

        ValueRef GenCmpEQ(const ValueRef& left, const ValueRef& right);
        ValueRef GenCmpNE(const ValueRef& left, const ValueRef& right);
        ValueRef GenCmpLE(const ValueRef& left, const ValueRef& right);
        ValueRef GenCmpGE(const ValueRef& left, const ValueRef& right);
        ValueRef GenAnd(const ValueRef& left, const ValueRef& right);
        ValueRef GenLogicalAnd(const ValueRef& left, const ValueRef& right);
        ValueRef GenOr(const ValueRef& left, const ValueRef& right);
        ValueRef GenLogicalOr(const ValueRef& left, const ValueRef& right);
        ValueRef GenXor(const ValueRef& left, const ValueRef& right);
        ValueRef GenCmpLT(const ValueRef& left, const ValueRef& right);
        ValueRef GenCmpGT(const ValueRef& left, const ValueRef& right);
        ValueRef GenShl(const ValueRef& left, const ValueRef& right);
        ValueRef GenAShr(const ValueRef& left, const ValueRef& right);
        ValueRef GenLShr(const ValueRef& left, const ValueRef& right);
        ValueRef GenAdd(const ValueRef& left, const ValueRef& right);
        ValueRef GenSub(const ValueRef& left, const ValueRef& right);
        ValueRef GenMul(const ValueRef& left, const ValueRef& right);
        ValueRef GenDiv(const ValueRef& left, const ValueRef& right);
        ValueRef GenRem(const ValueRef& left, const ValueRef& right);
        ValueRef GenNeg(const ValueRef& value);
        ValueRef GenNot(const ValueRef& value);
        ValueRef GenInv(const ValueRef& value);
        ValueRef GenInc(const ValueRef& value, bool opRight);
        ValueRef GenDec(const ValueRef& value, bool opRight);

        std::pair<ValueRef, ValueRef> CastToBestOf(const ValueRef& left, const ValueRef& right);

        // std::unique_ptr<CSawJIT> m_JIT;

        std::unique_ptr<llvm::LLVMContext> m_Context;
        std::unique_ptr<llvm::IRBuilder<>> m_Builder;
        std::unique_ptr<llvm::Module> m_Module;

        std::unique_ptr<llvm::FunctionPassManager> m_FPM;
        std::unique_ptr<llvm::LoopAnalysisManager> m_LAM;
        std::unique_ptr<llvm::FunctionAnalysisManager> m_FAM;
        std::unique_ptr<llvm::CGSCCAnalysisManager> m_CGAM;
        std::unique_ptr<llvm::ModuleAnalysisManager> m_MAM;
        std::unique_ptr<llvm::PassInstrumentationCallbacks> m_PIC;
        std::unique_ptr<llvm::StandardInstrumentations> m_SI;

        std::map<std::string, std::vector<FunctionRef>> m_Functions;
        std::map<std::string, ValueRef> m_GlobalValues;
        std::map<std::string, ValueRef> m_Values;
    };
}
