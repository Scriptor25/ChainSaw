#pragma once

#include <map>
#include <memory>
#include <csaw/Type.hpp>
#include <csaw/codegen/Def.hpp>
#include <csaw/lang/Def.hpp>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
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

        llvm::LLVMContext& GetContext() const;
        llvm::IRBuilder<>& GetBuilder() const;
        llvm::Module& GetModule() const;

        void Build() const;
        int Main(int argc, const char** argv);

        llvm::AllocaInst* CreateAlloca(llvm::Type* type, llvm::Value* arraySize = nullptr) const;

        void Gen(const StatementPtr& ptr);
        llvm::Type* Gen(const TypePtr& type) const;

    private:
        RValuePtr Cast(const ValuePtr& value, const TypePtr& type) const;
        std::pair<RValuePtr, RValuePtr> CastToBestOf(const RValuePtr& left, const RValuePtr& right) const;

        std::pair<Signature, llvm::Function*> FindFunction(const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args) const;

        void Gen(const ScopeStatement& statement);
        void Gen(const ForStatement& statement);
        void Gen(const FunctionStatement& statement);
        void Gen(const IfStatement& statement);
        void Gen(const RetStatement& statement);
        void Gen(const DefStatement& statement) const;
        void Gen(const VariableStatement& statement);
        void Gen(const WhileStatement& statement);

        ValuePtr Gen(const ExpressionPtr& ptr);
        RValuePtr Gen(const BinaryExpression& expression);
        RValuePtr Gen(const CallExpression& expression);
        RValuePtr Gen(const CastExpression& expression);
        RValuePtr Gen(const CharExpression& expression) const;
        LValuePtr Gen(const DereferenceExpression& expression);
        RValuePtr Gen(const FloatExpression& expression) const;
        LValuePtr Gen(const IdentifierExpression& expression);
        LValuePtr Gen(const IndexExpression& expression);
        RValuePtr Gen(const IntExpression& expression) const;
        LValuePtr Gen(const MemberExpression& expression);
        RValuePtr Gen(const ReferenceExpression& expression);
        ValuePtr Gen(const SelectExpression& expression);
        RValuePtr Gen(const StringExpression& expression) const;
        RValuePtr Gen(const UnaryExpression& expression);

        RValuePtr GenCmpEQ(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenCmpNE(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenCmpLE(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenCmpGE(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenAnd(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenLogicalAnd(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenOr(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenLogicalOr(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenXor(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenCmpLT(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenCmpGT(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenShl(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenAShr(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenLShr(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenAdd(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenSub(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenMul(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenDiv(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenRem(const RValuePtr& left, const RValuePtr& right) const;
        RValuePtr GenNeg(const RValuePtr& value) const;
        RValuePtr GenNot(const RValuePtr& value) const;
        RValuePtr GenInv(const RValuePtr& value) const;
        RValuePtr GenInc(const RValuePtr& value) const;
        RValuePtr GenDec(const RValuePtr& value) const;

        llvm::ExitOnError m_Error;
        std::unique_ptr<llvm::orc::LLJIT> m_JIT;

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

        std::map<std::string, LValuePtr> m_GlobalValues;
        std::map<std::string, LValuePtr> m_Values;

        llvm::Function* m_GlobalParent;
    };
}
