#include <filesystem>
#include <iostream>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Signature.hpp>
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

csaw::Expect<csaw::TypePtr> csaw::Builder::FromLLVM(const llvm::Type* type)
{
    if (type->isVoidTy()) return Type::GetVoid();
    if (type->isPointerTy()) return {PointerType::Get(Type::GetVoid())};
    if (type->isStructTy())
    {
        const auto ty = Type::Get(type->getStructName().str());
        if (!ty)
            return Expect<TypePtr>("Undefined struct type " + type->getStructName().str());
        return ty;
    }
    if (type->isFunctionTy())
    {
        std::vector<TypePtr> args;
        for (size_t i = 0; i < type->getFunctionNumParams(); ++i)
        {
            const auto arg = FromLLVM(type->getFunctionParamType(i));
            if (!arg)
                return Expect<TypePtr>("Function arg type is null: " + arg.Msg());
            args.push_back(arg.Get());
        }
        const bool is_vararg = type->isFunctionVarArg();
        const auto result = FromLLVM(llvm::dyn_cast<llvm::FunctionType>(type)->getReturnType());
        if (!result)
            return Expect<TypePtr>("Function result type is null: " + result.Msg());
        return {FunctionType::Get(result.Get(), args, is_vararg)};
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

    return Expect<TypePtr>("Unhandled llvm type");
}

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

void csaw::Builder::BeginModule(const std::string& name, const std::string& source_file)
{
    m_Context = std::make_unique<llvm::LLVMContext>();
    m_Builder = std::make_unique<llvm::IRBuilder<>>(*m_Context);
    m_Module = std::make_unique<llvm::Module>(name, *m_Context);
    m_Module->setSourceFileName(source_file);

    m_SI = std::make_unique<llvm::StandardInstrumentations>(*m_Context, true);
    m_SI->registerCallbacks(*m_PIC, m_MAM.get());

    const auto function_type = llvm::FunctionType::get(m_Builder->getVoidTy(), false);
    m_Global = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, name + ".global", *m_Module);
    const auto entry_block = llvm::BasicBlock::Create(*m_Context, "entry", m_Global);
    m_Builder->SetInsertPoint(entry_block);

    m_ScopeStack.clear();
    m_Values.clear();
}

void csaw::Builder::EndModule()
{
    m_Builder->CreateRetVoid();
    if (verifyFunction(*m_Global, &llvm::errs()))
    {
        std::cout << "Failed to verify global function" << std::endl;
        m_Global->viewCFG();
        m_Global->eraseFromParent();
        return;
    }

    // Optimize Global
    m_FPM->run(*m_Global, *m_FAM);

    // Append Global to CTORs
    appendToGlobalCtors(*m_Module, m_Global, 0);

    if (verifyModule(*m_Module))
    {
        std::cout << "Failed to verify module" << std::endl;
        return;
    }

    const auto name = m_Module->getName().str();
    m_Modules[name] = {std::move(m_Context), std::move(m_Module)};
}

int csaw::Builder::Output(const std::filesystem::path& dest_dir, const std::string& type)
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

    llvm::CodeGenFileType filetype = llvm::CodeGenFileType::CGFT_Null;
    std::string fileext;

    if (type == "obj")
    {
        filetype = llvm::CodeGenFileType::CGFT_ObjectFile;
        fileext = ".o";
    }
    else if (type == "asm")
    {
        filetype = llvm::CodeGenFileType::CGFT_AssemblyFile;
        fileext = ".s";
    }

    for (const auto& [name, pair] : m_Modules)
    {
        const auto filename = absolute(dest_dir / (name + fileext)).string();

        std::error_code error_code;
        llvm::raw_fd_ostream dest(filename, error_code, llvm::sys::fs::OF_None);

        if (error_code)
        {
            std::cout << "Failed to open output file '" << filename << "': " << error_code.message() << std::endl;
            return -1;
        }

        llvm::legacy::PassManager pass;
        if (machine->addPassesToEmitFile(pass, dest, nullptr, filetype))
        {
            std::cout << "Failed to output to file '" << filename << "'" << std::endl;
            return -1;
        }

        pair.Module->setTargetTriple(triple);
        pair.Module->setDataLayout(data_layout);
        pass.run(*pair.Module);

        dest.flush();
    }

    return 0;
}

int csaw::Builder::RunJIT(const std::string& entry_name, const int argc, const char** argv)
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
    auto entry_err = jit->lookup(entry_name);
    if (auto err = entry_err.takeError())
    {
        std::cout << "Failed to find entry function '" << entry_name << "'" << std::endl;
        return -1;
    }

    // Call entry function
    const auto entry_fn = entry_err->toPtr<int(*)(int, const char**)>();
    return entry_fn(argc, argv);
}

llvm::AllocaInst* csaw::Builder::CreateAlloca(llvm::Type* type, llvm::Value* array_size) const
{
    const auto block = m_Builder->GetInsertBlock();
    m_Builder->SetInsertPointPastAllocas(block->getParent());
    const auto inst = m_Builder->CreateAlloca(type, array_size);
    m_Builder->SetInsertPoint(block);
    return inst;
}

std::pair<csaw::Signature, llvm::Function*> csaw::Builder::FindFunction(const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args) const
{
    for (auto& function : m_Module->functions())
    {
        const auto s = Signature::Demangle(function);
        if (s.Name != name) continue;
        if (s.Parent != parent) continue;
        if (s.Args.size() > args.size() || (!s.IsVarargs && s.Args.size() != args.size())) continue;
        size_t i = 0;
        for (; i < s.Args.size(); ++i)
            if (!s.Args[i]->ParentOf(args[i]))
                break;
        if (i < s.Args.size()) continue;
        return {s, &function};
    }

    return {{name, parent, nullptr, args, false}, nullptr};
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
