#pragma once

#include <filesystem>
#include <csaw/Def.hpp>
#include <csaw/Expect.hpp>
#include <csaw/Signature.hpp>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/StandardInstrumentations.h>

namespace csaw
{
    struct ModuleData
    {
        std::unique_ptr<llvm::LLVMContext> Context = nullptr;
        std::unique_ptr<llvm::IRBuilder<>> Builder = nullptr;
        std::unique_ptr<llvm::Module> Module = nullptr;
        llvm::Function* Global = nullptr;
    };

    typedef std::pair<RValuePtr, RValuePtr> RValPair;

    class Builder
    {
    public:
        Builder();

        [[nodiscard]] llvm::LLVMContext& GetContext() const;
        [[nodiscard]] llvm::IRBuilder<>& GetBuilder() const;
        [[nodiscard]] llvm::Module& GetModule() const;
        [[nodiscard]] llvm::Function* GetGlobal() const;

        void BeginModule(const std::string& name, const std::string& source_file);
        int EndModule(const std::string& emit_ir_directory);

        int LinkModules();
        [[nodiscard]] int OutputModules(const std::string& output_file, llvm::CodeGenFileType output_type) const;

        [[nodiscard]] static int EmitIR(const llvm::Module& module, const std::string& output_directory);
        [[nodiscard]] static int Output(llvm::Module& module, const std::string& output_file, llvm::CodeGenFileType output_type);

        int RunJIT(int argc, const char** argv, const char** env);

        llvm::AllocaInst* CreateAlloca(llvm::Type* type, llvm::Value* array_size = nullptr) const;

        void Gen(const StatementPtr& ptr);
        [[nodiscard]] Expect<llvm::Type*> Gen(const TypePtr& type) const;

    private:
        [[nodiscard]] std::pair<llvm::Function*, Signature> FindFunction(const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args) const;
        void PushScopeStack();
        void PopScopeStack();

        int NextCtor();

        [[nodiscard]] Expect<RValuePtr> Cast(const ValuePtr& value, const TypePtr& type) const;
        [[nodiscard]] Expect<RValPair> CastToBestOf(const RValuePtr& left, const RValuePtr& right) const;

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
        [[nodiscard]] RValuePtr Gen(const CharExpression& expression) const;
        LValuePtr Gen(const DereferenceExpression& expression);
        [[nodiscard]] RValuePtr Gen(const FloatExpression& expression) const;
        ValuePtr Gen(const IdentifierExpression& expression);
        LValuePtr Gen(const IndexExpression& expression);
        [[nodiscard]] RValuePtr Gen(const IntExpression& expression) const;
        LValuePtr Gen(const MemberExpression& expression);
        RValuePtr Gen(const ReferenceExpression& expression);
        ValuePtr Gen(const SelectExpression& expression);
        [[nodiscard]] RValuePtr Gen(const SizeOfExpression& expression) const;
        [[nodiscard]] RValuePtr Gen(const StringExpression& expression) const;
        RValuePtr Gen(const UnaryExpression& expression);

        [[nodiscard]] RValuePtr GenCmpEQ(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenCmpNE(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenCmpLE(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenCmpGE(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenAnd(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenLogicalAnd(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenOr(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenLogicalOr(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenXor(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenCmpLT(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenCmpGT(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenShl(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenAShr(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenLShr(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenAdd(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenSub(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenMul(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenDiv(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenRem(const RValuePtr& left, const RValuePtr& right) const;
        [[nodiscard]] RValuePtr GenNeg(const RValuePtr& value) const;
        [[nodiscard]] RValuePtr GenNot(const RValuePtr& value) const;
        [[nodiscard]] RValuePtr GenInv(const RValuePtr& value) const;
        [[nodiscard]] RValuePtr GenInc(const RValuePtr& value) const;
        [[nodiscard]] RValuePtr GenDec(const RValuePtr& value) const;

        std::map<std::string, ModuleData> m_Modules;

        std::unique_ptr<llvm::FunctionPassManager> m_FPM;
        std::unique_ptr<llvm::LoopAnalysisManager> m_LAM;
        std::unique_ptr<llvm::FunctionAnalysisManager> m_FAM;
        std::unique_ptr<llvm::CGSCCAnalysisManager> m_CGAM;
        std::unique_ptr<llvm::ModuleAnalysisManager> m_MAM;
        std::unique_ptr<llvm::PassInstrumentationCallbacks> m_PIC;
        std::unique_ptr<llvm::StandardInstrumentations> m_SI;

        ModuleData m_ModuleData;
        std::map<llvm::Function*, Signature> m_Signatures;
        std::vector<std::map<std::string, ValuePtr>> m_ScopeStack;
        std::map<std::string, ValuePtr> m_Values;
        int m_CtorPriority;
    };
}
