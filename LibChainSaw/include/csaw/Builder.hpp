#pragma once

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

    typedef std::pair<RValuePtr, RValuePtr> RValuePair;

    class Builder
    {
    public:
        explicit Builder(bool obfusecate);

        [[nodiscard]] llvm::LLVMContext& GetContext() const;
        [[nodiscard]] llvm::IRBuilder<>& GetBuilder() const;
        [[nodiscard]] llvm::Module& GetModule() const;
        [[nodiscard]] llvm::Function* GetGlobal() const;

        void BeginModule(const std::string& name, const std::string& source_file);
        int EndModule(const std::string& emit_ir_directory);

        int LinkModules();
        [[nodiscard]] int OutputModules(const std::string& output_file, llvm::CodeGenFileType output_type) const;

        static int EmitIR(const llvm::Module& module, const std::string& output_directory);
        static int Output(llvm::Module& module, const std::string& output_file, llvm::CodeGenFileType output_type);

        int RunJIT(int argc, const char** argv, const char** env);

        llvm::AllocaInst* CreateAlloca(llvm::Type* type, llvm::Value* array_size = nullptr) const;

        void Gen(const StatementPtr& ptr);
        [[nodiscard]] Expect<llvm::Type*> Gen(const TypePtr& type) const;

        Expect<RValuePtr> Cast(const ValuePtr& value, const TypePtr& type);
        Expect<RValuePair> CastToBestOf(const RValuePtr& left, const RValuePtr& right);

    private:
        llvm::Function* FindFunctionBySignature(const Signature& sig);
        std::pair<llvm::Function*, Signature> FindBestFunctionBySignature(const Signature& sig);
        Expect<RValuePtr> CreateCall(const ValuePtr& callee, const LValuePtr& parent, const std::vector<ValuePtr>& args);
        Expect<LValuePtr> CreateCtorCall(const ValuePtr& callee, const TypePtr& ctortype, const std::vector<ValuePtr>& args);
        Expect<RValuePtr> FindBestAndCall(const std::string& name, const LValuePtr& parent, const std::vector<ValuePtr>& args);
        Expect<LValuePtr> FindBestCtorAndCall(const std::string& name, const std::vector<ValuePtr>& args);
        [[nodiscard]] std::map<llvm::Function*, Signature> FindFunctions(const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args, bool is_ctor) const;
        [[nodiscard]] std::pair<llvm::Function*, Signature> FindBestFunction(const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args, bool is_ctor = false) const;
        static int RankFunction(const Signature& signature, const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args);

        void PushScopeStack();
        void PopScopeStack();

        void Gen(const ScopeStatement& statement);
        void Gen(const ForStatement& statement);
        void Gen(const FunctionStatement& statement);
        void Gen(const IfStatement& statement);
        void Gen(const RetStatement& statement);
        void Gen(const DefStatement& statement);
        void Gen(const VariableStatement& statement);
        void Gen(const WhileStatement& statement);

        ValuePtr Gen(const ExpressionPtr& ptr, const TypePtr& expected);
        RValuePtr Gen(const BinaryExpression& expression);
        ValuePtr Gen(const CallExpression& expression, const TypePtr& expected);
        RValuePtr Gen(const CastExpression& expression);
        RValuePtr Gen(const CharExpression& expression);
        LValuePtr Gen(const DereferenceExpression& expression);
        RValuePtr Gen(const FloatExpression& expression, const TypePtr& expected);
        ValuePtr Gen(const IdentifierExpression& expression, const TypePtr& expected);
        LValuePtr Gen(const IndexExpression& expression);
        RValuePtr Gen(const IntExpression& expression, const TypePtr& expected);
        ValuePtr Gen(const MemberExpression& expression, const TypePtr& expected);
        RValuePtr Gen(const ReferenceExpression& expression, const TypePtr& expected);
        ValuePtr Gen(const SelectExpression& expression, const TypePtr& expected);
        RValuePtr Gen(const SizeOfExpression& expression);
        RValuePtr Gen(const StringExpression& expression);
        RValuePtr Gen(const UnaryExpression& expression);

        RValuePtr GenCmpEQ(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenCmpNE(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenCmpLE(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenCmpGE(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenAnd(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenLogicalAnd(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenOr(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenLogicalOr(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenXor(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenCmpLT(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenCmpGT(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenShl(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenAShr(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenLShr(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenAdd(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenSub(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenMul(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenDiv(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenRem(const RValuePtr& left, const RValuePtr& right);
        RValuePtr GenNeg(const RValuePtr& value);
        RValuePtr GenNot(const RValuePtr& value);
        RValuePtr GenInv(const RValuePtr& value);
        RValuePtr GenInc(const RValuePtr& value);
        RValuePtr GenDec(const RValuePtr& value);

        bool m_Obfusecate;
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
    };
}
