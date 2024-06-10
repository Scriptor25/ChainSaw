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

    typedef std::pair<ValuePtr, ValuePtr> ValuePair;

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

        Expect<ValuePtr> Cast(const ValuePtr& value, const TypePtr& type);
        Expect<ValuePair> CastToBestOf(const ValuePtr& left, const ValuePtr& right);

        std::map<std::string, std::vector<TypePtr>> GetSymbols();

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
        void Gen(const DefStatement& statement) const;
        void Gen(const VariableStatement& statement);
        void Gen(const WhileStatement& statement);

        ValuePtr Gen(const ExpressionPtr& ptr, const TypePtr& expected);
        ValuePtr Gen(const BinaryExpression& expression);
        ValuePtr Gen(const CallExpression& expression, const TypePtr& expected);
        ValuePtr Gen(const CastExpression& expression);
        ValuePtr Gen(const CharExpression& expression);
        ValuePtr Gen(const DereferenceExpression& expression);
        ValuePtr Gen(const FloatExpression& expression, const TypePtr& expected);
        ValuePtr Gen(const IdentifierExpression& expression, const TypePtr& expected);
        ValuePtr Gen(const IndexExpression& expression);
        ValuePtr Gen(const IntExpression& expression, const TypePtr& expected);
        ValuePtr Gen(const MemberExpression& expression, const TypePtr& expected);
        ValuePtr Gen(const ReferenceExpression& expression, const TypePtr& expected);
        ValuePtr Gen(const SelectExpression& expression, const TypePtr& expected);
        ValuePtr Gen(const SizeOfExpression& expression);
        ValuePtr Gen(const StringExpression& expression);
        ValuePtr Gen(const UnaryExpression& expression);

        ValuePtr GenCmpEQ(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenCmpNE(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenCmpLE(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenCmpGE(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenAnd(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenLogicalAnd(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenOr(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenLogicalOr(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenXor(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenCmpLT(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenCmpGT(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenShl(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenAShr(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenLShr(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenAdd(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenSub(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenMul(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenDiv(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenRem(const ValuePtr& left, const ValuePtr& right);
        ValuePtr GenNeg(const ValuePtr& value);
        ValuePtr GenNot(const ValuePtr& value);
        ValuePtr GenInv(const ValuePtr& value);
        ValuePtr GenInc(const ValuePtr& value);
        ValuePtr GenDec(const ValuePtr& value);

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
