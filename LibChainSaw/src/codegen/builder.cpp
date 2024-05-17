#include <iostream>
#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Signature.hpp>
#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

csaw::Builder* csaw::Builder::Create()
{
    try { return new Builder(); }
    catch (const ChainSawMessage& message)
    {
        std::cout << "Failed to create builder: " << message.Message << std::endl;
        return nullptr;
    }
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
        return FunctionType::Get(result, args, is_vararg);
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

int csaw::Builder::BeginModule(const std::string& name, const std::string& source_file)
{
    m_ModuleNames.push_back(name);

    m_Context = std::make_unique<llvm::LLVMContext>();
    m_Builder = std::make_unique<llvm::IRBuilder<>>(*m_Context);
    m_Module = std::make_unique<llvm::Module>(name, *m_Context);

    m_Module->setSourceFileName(source_file);
    m_Module->setDataLayout(m_JIT->getDataLayout());
    m_Module->setTargetTriple(m_JIT->getTargetTriple().str());

    m_SI = std::make_unique<llvm::StandardInstrumentations>(*m_Context, true);
    m_SI->registerCallbacks(*m_PIC, m_MAM.get());

    const auto function_type = llvm::FunctionType::get(m_Builder->getVoidTy(), false);
    m_GlobalParent = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, name + ".global", *m_Module);
    const auto entry_block = llvm::BasicBlock::Create(*m_Context, "entry", m_GlobalParent);
    m_Builder->SetInsertPoint(entry_block);

    m_ScopeStack.clear();
    m_Values.clear();

    return 0;
}

int csaw::Builder::EndModule()
{
    const std::string name = m_Module->getName().str();

    m_Builder->CreateRetVoid();
    if (verifyFunction(*m_GlobalParent, &llvm::errs()))
    {
        std::cout << "Failed to verify global function from module '" << name << "'" << std::endl;
        m_GlobalParent->viewCFG();
        m_GlobalParent->eraseFromParent();
        m_Module.reset();
        return -1;
    }
    m_FPM->run(*m_GlobalParent, *m_FAM);

    if (verifyModule(*m_Module))
    {
        std::cout << "Failed to verify module '" << name << "'" << std::endl;
        m_Module.reset();
        return -1;
    }

    if (auto err = m_JIT->addIRModule(llvm::orc::ThreadSafeModule(std::move(m_Module), std::move(m_Context))))
    {
        std::cout << "Failed to add module '" << name << "' to JIT: " << std::endl;
        logAllUnhandledErrors(std::move(err), llvm::errs());
        m_Module.reset();
        return -1;
    }

    m_Module.reset();
    return 0;
}

int csaw::Builder::RunJIT(const std::string& entry_name, const int argc, const char** argv) const
{
    for (const auto& name : m_ModuleNames)
    {
        auto global = m_JIT->lookup(name + ".global");
        if (auto err = global.takeError())
        {
            std::cout << "Failed to find global function in module '" << name << "'" << std::endl;
            logAllUnhandledErrors(std::move(err), llvm::errs());
            return -1;
        }

        const auto global_fn = global->toPtr<void(*)()>();
        global_fn();
    }

    auto entry = m_JIT->lookup(entry_name);
    if (auto err = entry.takeError())
    {
        Signature signature;
        signature.IsC = false;
        signature.Name = entry_name;
        signature.Result = Type::GetInt32();
        auto entry_sig = signature.Mangle();

        entry = m_JIT->lookup(entry_sig);
        if (auto err1 = entry.takeError())
        {
            signature.Args = {Type::GetInt32(), PointerType::Get(PointerType::Get(Type::GetInt8()))};
            entry_sig = signature.Mangle();

            entry = m_JIT->lookup(entry_sig);
            if (auto err2 = entry.takeError())
            {
                std::cout << "Failed to find entry function '" << entry_name << "'" << std::endl;
                return -1;
            }
        }
    }

    const auto entry_fn = entry->toPtr<int(*)(int, const char**)>();
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

csaw::Builder::Builder()
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    llvm::orc::LLJITBuilder builder;
    builder.setLinkProcessSymbolsByDefault(true);
    auto jit = builder.create();
    if (auto err = jit.takeError())
    {
        logAllUnhandledErrors(std::move(err), llvm::errs());
        CSAW_MESSAGE_NONE(false, "Failed to create the JIT");
    }
    m_JIT = std::move(*jit);

    llvm::orc::SymbolMap symbols;
    symbols[m_JIT->mangleAndIntern("fprintf")] = {
        llvm::orc::ExecutorAddr(llvm::pointerToJITTargetAddress(&fprintf)),
        llvm::JITSymbolFlags()
    };
    if (auto err = m_JIT->getMainJITDylib().define(absoluteSymbols(symbols)))
    {
        logAllUnhandledErrors(std::move(err), llvm::errs());
        CSAW_MESSAGE_NONE(false, "Failed to define symbol map in the JITs main dylib");
    }

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

    if (!testing && !alt.empty())
    {
        std::cout << "Alternative signatures: " << std::endl;
        for (const auto& s : alt)
            std::cout << s.Mangle(true) << std::endl;
    }

    return {Signature(name, parent, nullptr, args, false), nullptr};
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
