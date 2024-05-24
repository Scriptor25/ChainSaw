#include <filesystem>
#include <iostream>
#include <csaw/Builder.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Type.hpp>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Transforms/Utils/ModuleUtils.h>

csaw::Builder::Builder()
{
    m_FPM = std::make_unique<llvm::FunctionPassManager>();
    m_LAM = std::make_unique<llvm::LoopAnalysisManager>();
    m_FAM = std::make_unique<llvm::FunctionAnalysisManager>();
    m_CGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
    m_MAM = std::make_unique<llvm::ModuleAnalysisManager>();
    m_PIC = std::make_unique<llvm::PassInstrumentationCallbacks>();

    m_FPM->addPass(llvm::InstCombinePass());
    m_FPM->addPass(llvm::ReassociatePass());
    m_FPM->addPass(llvm::GVNPass());
    m_FPM->addPass(llvm::SimplifyCFGPass());

    llvm::PassBuilder pb;
    pb.registerModuleAnalyses(*m_MAM);
    pb.registerFunctionAnalyses(*m_FAM);
    pb.crossRegisterProxies(*m_LAM, *m_FAM, *m_CGAM, *m_MAM);
}

llvm::LLVMContext& csaw::Builder::GetContext() const
{
    return *m_ModuleData.Context;
}

llvm::IRBuilder<>& csaw::Builder::GetBuilder() const
{
    return *m_ModuleData.Builder;
}

llvm::Module& csaw::Builder::GetModule() const
{
    return *m_ModuleData.Module;
}

llvm::Function* csaw::Builder::GetGlobal() const
{
    return m_ModuleData.Global;
}

void csaw::Builder::BeginModule(const std::string& name, const std::string& source_file)
{
    m_ModuleData = {};
    m_Signatures.clear();
    m_ScopeStack.clear();
    m_Values.clear();

    m_ModuleData.Context = std::make_unique<llvm::LLVMContext>();
    m_ModuleData.Builder = std::make_unique<llvm::IRBuilder<>>(GetContext());
    m_ModuleData.Module = std::make_unique<llvm::Module>(name, GetContext());

    GetModule().setSourceFileName(source_file);

    m_SI = std::make_unique<llvm::StandardInstrumentations>(GetContext(), true);
    m_SI->registerCallbacks(*m_PIC, m_MAM.get());

    const auto function_type = llvm::FunctionType::get(GetBuilder().getVoidTy(), false);
    m_ModuleData.Global = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, name + ".global", GetModule());
    const auto entry_block = llvm::BasicBlock::Create(GetContext(), "entry", GetGlobal());
    m_ModuleData.Builder->SetInsertPoint(entry_block);
}

void csaw::Builder::EndModule(const std::string& output_file, const llvm::CodeGenFileType output_type, const std::string& emit_ir_file)
{
    GetBuilder().CreateRetVoid();
    if (verifyFunction(*GetGlobal(), &llvm::errs()))
    {
        std::cout << "Failed to verify global function" << std::endl;
        GetGlobal()->viewCFG();
        GetGlobal()->eraseFromParent();
        return;
    }

    // Optimize Global
    m_FPM->run(*GetGlobal(), *m_FAM);

    // Append Global to CTORs
    appendToGlobalCtors(GetModule(), GetGlobal(), 0);

    if (verifyModule(GetModule()))
    {
        std::cout << "Failed to verify module" << std::endl;
        m_ModuleData = {};
        return;
    }

    if (!emit_ir_file.empty())
        (void)EmitIR(GetModule(), emit_ir_file);

    if (!output_file.empty())
        (void)Output(GetModule(), output_file, output_type);

    const auto name = GetModule().getName().str();
    m_Modules[name] = std::move(m_ModuleData);
    m_ModuleData = {};
}

int csaw::Builder::EmitIR(const llvm::Module& module, const std::string& output_file)
{
    std::error_code error_code;
    llvm::raw_fd_ostream dest(output_file, error_code, llvm::sys::fs::OF_None);
    if (error_code)
    {
        std::cout << "Failed to open output file '" << output_file << "': " << error_code.message() << std::endl;
        return -1;
    }

    module.print(dest, nullptr, false, true);
    dest.flush();

    return 0;
}

int csaw::Builder::Output(llvm::Module& module, const std::string& output_file, const llvm::CodeGenFileType output_type)
{
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    const auto triple = llvm::sys::getDefaultTargetTriple();

    std::string err;
    const auto target = llvm::TargetRegistry::lookupTarget(triple, err);
    if (!target)
    {
        std::cout << "Failed to get target for triple: " << err << std::endl;
        return -1;
    }

    const auto cpu = "generic";
    const auto features = "";

    llvm::TargetOptions options;
    const auto machine = target->createTargetMachine(triple, cpu, features, options, llvm::Reloc::PIC_);
    const auto data_layout = machine->createDataLayout();

    std::error_code error_code;
    llvm::raw_fd_ostream dest(output_file, error_code, llvm::sys::fs::OF_None);
    if (error_code)
    {
        std::cout << "Failed to open output file '" << output_file << "': " << error_code.message() << std::endl;
        return -1;
    }

    llvm::legacy::PassManager pass;
    if (machine->addPassesToEmitFile(pass, dest, nullptr, output_type))
    {
        std::cout << "Failed to output to file '" << output_file << "'" << std::endl;
        return -1;
    }

    module.setTargetTriple(triple);
    module.setDataLayout(data_layout);

    pass.run(module);
    dest.flush();

    return 0;
}

int csaw::Builder::RunJIT(const int argc, const char** argv, const char** env)
{
    // Initialize Target
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    // Create LLJIT
    llvm::orc::LLJITBuilder builder;
    builder.setLinkProcessSymbolsByDefault(true);
    auto jit_err = builder.create();
    if (auto err = jit_err.takeError())
    {
        logAllUnhandledErrors(std::move(err), llvm::errs());
        return -1;
    }

    const std::unique_ptr<llvm::orc::LLJIT> jit = std::move(*jit_err);

    // Link symbols into main dylib
    llvm::orc::SymbolMap symbols;
    // for some reason fprintf does not work correctly if you dont specify it here
    symbols[jit->mangleAndIntern("fprintf")] = {
        llvm::orc::ExecutorAddr(llvm::pointerToJITTargetAddress(&fprintf)),
        llvm::JITSymbolFlags()
    };

    if (auto err = jit->getMainJITDylib().define(absoluteSymbols(symbols)))
    {
        logAllUnhandledErrors(std::move(err), llvm::errs());
        return -1;
    }

    // Add all modules to jit
    for (auto& [name, pair] : m_Modules)
    {
        pair.Module->setDataLayout(jit->getDataLayout());
        pair.Module->setTargetTriple(jit->getTargetTriple().getTriple());

        if (auto err = jit->addIRModule(llvm::orc::ThreadSafeModule(std::move(pair.Module), std::move(pair.Context))))
        {
            std::cout << "Failed to add module '" << name << "': " << std::endl;
            logAllUnhandledErrors(std::move(err), llvm::errs());
        }
    }

    for (auto& [name, pair] : m_Modules)
    {
        auto global = jit->lookup(name + ".global");
        if (auto err = global.takeError())
        {
            std::cout << "Failed to find global function for module '" << name << "'" << std::endl;
            continue;
        }

        const auto global_fn = global->toPtr<void()>();
        global_fn();
    }

    // Find entry function
    auto entry_err = jit->lookup("main");
    if (auto err = entry_err.takeError())
    {
        std::cout << "Failed to find entry function" << std::endl;
        return -1;
    }

    // Call entry function
    const auto entry_fn = entry_err->toPtr<int(*)(int, const char**, const char**)>();
    return entry_fn(argc, argv, env);
}

llvm::AllocaInst* csaw::Builder::CreateAlloca(llvm::Type* type, llvm::Value* array_size) const
{
    const auto block = GetBuilder().GetInsertBlock();
    GetBuilder().SetInsertPointPastAllocas(block->getParent());
    const auto inst = GetBuilder().CreateAlloca(type, array_size);
    GetBuilder().SetInsertPoint(block);
    return inst;
}

std::pair<llvm::Function*, csaw::Signature> csaw::Builder::FindFunction(const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args) const
{
    for (const auto& [f, s] : m_Signatures)
    {
        if (s.Name != name) continue;
        if (s.Parent != parent) continue;
        if (s.Args.size() > args.size() || (!s.IsVarargs && s.Args.size() != args.size())) continue;
        size_t i = 0;
        for (; i < s.Args.size(); ++i)
            if (!s.Args[i]->ParentOf(args[i]))
                break;
        if (i < s.Args.size()) continue;
        return {f, s};
    }

    return {nullptr, {name, parent, nullptr, args, false}};
}

void csaw::Builder::PushScopeStack()
{
    m_ScopeStack.push_back(m_Values);
}

void csaw::Builder::PopScopeStack()
{
    m_Values = m_ScopeStack.back();
    m_ScopeStack.pop_back();
}
