#include <iostream>
#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Signature.hpp>
#include <csaw/codegen/Value.hpp>
#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

csaw::Builder::Builder(const std::string& moduleName)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    llvm::orc::LLJITBuilder builder;
    builder.setLinkProcessSymbolsByDefault(true);
    m_JIT = m_Error(builder.create());

    llvm::orc::SymbolMap symbols;
    symbols[m_JIT->mangleAndIntern("fprintf")] = {
        llvm::orc::ExecutorAddr(llvm::pointerToJITTargetAddress(&fprintf)),
        llvm::JITSymbolFlags()
    };
    m_Error(m_JIT->getMainJITDylib().define(absoluteSymbols(symbols)));

    m_Context = std::make_unique<llvm::LLVMContext>();
    m_Builder = std::make_unique<llvm::IRBuilder<>>(*m_Context);
    m_Module = std::make_unique<llvm::Module>(moduleName, *m_Context);
    m_Module->setDataLayout(m_JIT->getDataLayout());
    m_Module->setTargetTriple(m_JIT->getTargetTriple().str());

    m_FPM = std::make_unique<llvm::FunctionPassManager>();
    m_LAM = std::make_unique<llvm::LoopAnalysisManager>();
    m_FAM = std::make_unique<llvm::FunctionAnalysisManager>();
    m_CGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
    m_MAM = std::make_unique<llvm::ModuleAnalysisManager>();
    m_PIC = std::make_unique<llvm::PassInstrumentationCallbacks>();
    m_SI = std::make_unique<llvm::StandardInstrumentations>(*m_Context, true);
    m_SI->registerCallbacks(*m_PIC, m_MAM.get());

    m_FPM->addPass(llvm::InstCombinePass());
    m_FPM->addPass(llvm::ReassociatePass());
    m_FPM->addPass(llvm::GVNPass());
    m_FPM->addPass(llvm::SimplifyCFGPass());

    llvm::PassBuilder pb;
    pb.registerModuleAnalyses(*m_MAM);
    pb.registerFunctionAnalyses(*m_FAM);
    pb.crossRegisterProxies(*m_LAM, *m_FAM, *m_CGAM, *m_MAM);

    const auto function_type = llvm::FunctionType::get(m_Builder->getVoidTy(), false);
    m_GlobalParent = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, "__global__", *m_Module);
    const auto entry_block = llvm::BasicBlock::Create(*m_Context, "entry", m_GlobalParent);
    m_Builder->SetInsertPoint(entry_block);
}

llvm::LLVMContext& csaw::Builder::GetContext() const
{
    return *m_Context;
}

llvm::IRBuilder<>& csaw::Builder::GetBuilder() const
{
    return *m_Builder;
}

llvm::Module& csaw::Builder::GetModule() const
{
    return *m_Module;
}

void csaw::Builder::Build() const
{
    m_Builder->CreateRetVoid();
    if (verifyFunction(*m_GlobalParent, &llvm::errs()))
    {
        m_GlobalParent->viewCFG();
        m_GlobalParent->eraseFromParent();
        CSAW_MESSAGE_NONE(true, "failed to verify __global__");
    }
    m_FPM->run(*m_GlobalParent, *m_FAM);
}

int csaw::Builder::Main(const int argc, const char** argv)
{
    m_Error(m_JIT->addIRModule(llvm::orc::ThreadSafeModule(std::move(m_Module), std::move(m_Context))));

    const auto global_fn = m_Error(m_JIT->lookup("__global__")).toPtr<void(*)()>();
    global_fn();

    const auto main_fn = m_Error(m_JIT->lookup("main")).toPtr<int(*)(int, const char**)>();
    return main_fn(argc, argv);
}

llvm::AllocaInst* csaw::Builder::CreateAlloca(llvm::Type* type, llvm::Value* arraySize) const
{
    const auto block = m_Builder->GetInsertBlock();
    m_Builder->SetInsertPointPastAllocas(block->getParent());
    const auto inst = m_Builder->CreateAlloca(type, arraySize);
    m_Builder->SetInsertPoint(block);
    return inst;
}

csaw::TypePtr csaw::Builder::FromLLVM(const llvm::Type* type)
{
    if (type->isVoidTy()) return Type::GetVoid();
    if (type->isPointerTy()) return PointerType::Get(Type::GetVoid());
    if (type->isStructTy()) return Type::Get(type->getStructName().str());
    if (type->isFunctionTy())
    {
        std::vector<TypePtr> args;
        for (size_t i = 0; i < type->getFunctionNumParams(); ++i)
            args.push_back(FromLLVM(type->getFunctionParamType(i)));
        const bool is_vararg = type->isFunctionVarArg();
        const auto result = FromLLVM(llvm::dyn_cast<llvm::FunctionType>(type)->getReturnType());
        return FunctionType::Get(args, is_vararg, result);
    }
    if (type->isIntegerTy(1)) return Type::GetInt1();
    if (type->isIntegerTy(8)) return Type::GetInt8();
    if (type->isIntegerTy(16)) return Type::GetInt16();
    if (type->isIntegerTy(32)) return Type::GetInt32();
    if (type->isIntegerTy(64)) return Type::GetInt64();
    if (type->isIntegerTy(128)) return Type::GetInt128();
    if (type->isHalfTy()) return Type::GetFlt16();
    if (type->isFloatTy()) return Type::GetFlt32();
    if (type->isDoubleTy()) return Type::GetFlt64();
    CSAW_MESSAGE_NONE(true, "cannot get csaw type for llvm type");
}

std::pair<csaw::Signature, llvm::Function*> csaw::Builder::FindFunction(const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args, const bool testing) const
{
    std::vector<Signature> alt;
    for (auto& function : m_Module->functions())
    {
        const auto s = Signature::Demangle(function);
        if (s.Name != name) continue;
        alt.push_back(s);
        if (s.Parent != parent) continue;
        if (s.Args.size() > args.size() || (!s.IsVarargs && s.Args.size() != args.size())) continue;
        size_t i = 0;
        for (; i < s.Args.size(); ++i)
            if (!s.Args[i]->ParentOf(args[i]))
                break;
        if (i < s.Args.size()) continue;
        return {s, &function};
    }
    if (!testing)
    {
        std::cout << "Alternative signatures: " << std::endl;
        for (const auto& s : alt)
            std::cout << s.Mangle(true) << std::endl;
    }
    return {Signature(name, parent, nullptr, args, false), nullptr};
}
