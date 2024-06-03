#include <filesystem>
#include <iostream>
#include <csaw/Builder.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Linker/Linker.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>
#include <llvm/Transforms/Utils/Mem2Reg.h>
#include <llvm/Transforms/Utils/ModuleUtils.h>

csaw::Builder::Builder(const bool obfusecate)
    : m_Obfusecate(obfusecate)
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
    m_FPM->addPass(llvm::PromotePass());

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

int csaw::Builder::EndModule(const std::string& emit_ir_directory)
{
    if (!GetGlobal()->getEntryBlock().empty())
    {
        GetBuilder().CreateRetVoid();
        if (verifyFunction(*GetGlobal(), &llvm::errs()))
        {
            std::cout << "Failed to verify global function" << std::endl;
            GetGlobal()->viewCFG();
            GetGlobal()->eraseFromParent();
            return 1;
        }

        // Optimize Global
        m_FPM->run(*GetGlobal(), *m_FAM);

        // Append Global to CTORs
        appendToGlobalCtors(GetModule(), GetGlobal(), 0);
    }
    else GetGlobal()->eraseFromParent();

    if (verifyModule(GetModule()))
    {
        std::cout << "Failed to verify module" << std::endl;
        m_ModuleData = {};
        return 1;
    }

    if (!emit_ir_directory.empty())
        (void)EmitIR(GetModule(), emit_ir_directory);

    const auto name = GetModule().getName().str();
    m_Modules[name] = std::move(m_ModuleData);
    m_ModuleData = {};

    return 0;
}

// DO NOT USE
int csaw::Builder::LinkModules()
{
    std::vector<ModuleData> modules;
    for (auto& [key, data] : m_Modules)
        modules.push_back(std::move(data));
    m_Modules.clear();

    auto module = std::move(modules[0]);
    llvm::Linker linker(*module.Module);

    int error = 0;
    for (size_t i = 1; i < modules.size(); ++i)
    {
        const auto name = modules[i].Module->getName().str();
        const int err = linker.linkInModule(std::move(modules[i].Module));
        if (err)
            std::cout << "Failed to link module '" << name << "'" << std::endl;
        error |= err;
    }

    m_Modules[module.Module->getName().str()] = std::move(module);

    return error;
}

int csaw::Builder::OutputModules(const std::string& output_file, const llvm::CodeGenFileType output_type) const
{
    if (m_Modules.empty())
    {
        std::cout << "Failed to output modules: no module present" << std::endl;
        return 1;
    }

    if (m_Modules.size() > 1)
    {
        // LinkModules();
        std::cout << "Failed to output modules: module linking not yet supported" << std::endl;
        return 1;
    }

    auto& module = *m_Modules.begin()->second.Module;
    (void)Output(module, output_file, output_type);

    return 0;
}

int csaw::Builder::EmitIR(const llvm::Module& module, const std::string& output_directory)
{
    const auto output_file = (std::filesystem::path(output_directory) / (module.getName().str() + ".ll")).string();

    std::error_code error_code;
    llvm::raw_fd_ostream dest(output_file, error_code, llvm::sys::fs::OF_None);
    if (error_code)
    {
        std::cout << "Failed to open output file '" << output_file << "': " << error_code.message() << std::endl;
        return 1;
    }

    module.print(dest, nullptr, false, true);
    dest.flush();

    return 0;
}

int csaw::Builder::Output(llvm::Module& module, const std::string& output_file, const llvm::CodeGenFileType output_type)
{
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllTargetMCAs();
    llvm::InitializeAllDisassemblers();

    const auto triple = llvm::sys::getDefaultTargetTriple();

    std::string err;
    const auto target = llvm::TargetRegistry::lookupTarget(triple, err);
    if (!target)
    {
        std::cout << "Failed to get target for triple: " << err << std::endl;
        return 1;
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
        return 1;
    }

    llvm::legacy::PassManager pass;
    if (machine->addPassesToEmitFile(pass, dest, nullptr, output_type))
    {
        std::cout << "Failed to output to file '" << output_file << "'" << std::endl;
        return 1;
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
    llvm::orc::LLJITBuilder lljit_builder;
    lljit_builder.setLinkProcessSymbolsByDefault(true);
    auto jit_err = lljit_builder.create();
    if (auto err = jit_err.takeError())
    {
        std::cout << "Failed to create JIT:" << std::endl;
        logAllUnhandledErrors(std::move(err), llvm::errs());
        return 1;
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
        std::cout << "Failed to register symbol table in JIT:" << std::endl;
        logAllUnhandledErrors(std::move(err), llvm::errs());
        return 1;
    }

    // Add all modules to jit
    if (m_Modules.empty())
    {
        std::cout << "Failed to add modules to JIT: no modules present" << std::endl;
        return 1;
    }

    std::vector<std::string> ctor_names;
    for (auto& [key, data] : m_Modules)
    {
        auto& [context, builder, module, global] = data;

        const auto name = module->getName().str();
        module->setDataLayout(jit->getDataLayout());
        module->setTargetTriple(jit->getTargetTriple().getTriple());

        if (const auto global_ctors = module->getNamedGlobal("llvm.global_ctors"))
        {
            const auto ctor_array = llvm::cast<llvm::ConstantArray>(global_ctors->getInitializer());
            for (unsigned i = 0; i < ctor_array->getNumOperands(); ++i)
            {
                const auto ctor_struct = llvm::cast<llvm::ConstantStruct>(ctor_array->getOperand(i));
                const auto ctor = ctor_struct->getOperand(1);

                if (const auto func = llvm::dyn_cast<llvm::Function>(ctor->stripPointerCasts()))
                    ctor_names.push_back(func->getName().str());
            }
        }

        if (auto err = jit->addIRModule(llvm::orc::ThreadSafeModule(std::move(module), std::move(context))))
        {
            std::cout << "Failed to add module '" << name << "' to JIT: " << std::endl;
            logAllUnhandledErrors(std::move(err), llvm::errs());
            return 1;
        }
    }

    for (const auto& ctor_name : ctor_names)
    {
        auto ctor_error = jit->lookup(ctor_name);
        if (auto err = ctor_error.takeError())
        {
            std::cout << "Failed to lookup ctor function '" << ctor_name << "':" << std::endl;
            logAllUnhandledErrors(std::move(err), llvm::errs());
            return 1;
        }

        const auto ctor_fn = ctor_error->toPtr<void()>();
        ctor_fn();
    }

    // Find entry function
    auto entry_err = jit->lookup("main");
    if (auto err = entry_err.takeError())
    {
        std::cout << "Failed to lookup entry function 'main':" << std::endl;
        logAllUnhandledErrors(std::move(err), llvm::errs());
        return 1;
    }

    // Call entry function
    const auto entry_fn = entry_err->toPtr<int(int, const char**, const char**)>();
    return entry_fn(argc, argv, env);
}

llvm::AllocaInst* csaw::Builder::CreateAlloca(llvm::Type* type, llvm::Value* array_size) const
{
    if (!type->isSized())
        return nullptr;

    const auto block = GetBuilder().GetInsertBlock();
    GetBuilder().SetInsertPointPastAllocas(block->getParent());
    const auto inst = GetBuilder().CreateAlloca(type, array_size);
    GetBuilder().SetInsertPoint(block);
    return inst;
}

llvm::Function* csaw::Builder::FindFunctionBySignature(const Signature& sig)
{
    for (const auto& [function, signature] : m_Signatures)
        if (sig == signature)
            return function;
    return nullptr;
}

std::pair<llvm::Function*, csaw::Signature> csaw::Builder::FindBestFunctionBySignature(const Signature& sig)
{
    int highscore = 0;
    std::pair<llvm::Function*, csaw::Signature> best;

    for (const auto& [function, signature] : m_Signatures)
    {
        if (signature == sig) return {function, signature};

        if (signature.Name != sig.Name) continue;
        if (signature.Parent != sig.Parent) continue;
        if (signature.Args.size() > sig.Args.size() || (!signature.IsVarargs && signature.Args.size() != sig.Args.size())) continue;

        size_t i = 0;
        for (; i < signature.Args.size(); ++i)
            if (!signature.Args[i]->ParentOf(sig.Args[i]))
                break;
        if (i < signature.Args.size()) continue;

        int score = 0;
        for (i = 0; i < signature.Args.size(); ++i)
        {
            if (signature.Args[i] == sig.Args[i])
                score += 100;
            else if (signature.Args[i]->ParentOf(sig.Args[i]))
                score += 50;
        }

        score += static_cast<int>(sig.Args.size() - (sig.Args.size() - signature.Args.size()) - 1) * 10;
        if (signature.IsVarargs == sig.IsVarargs) score += 200;
        if (signature.Result == sig.Result) score += 200;

        if (score >= highscore)
        {
            highscore = score;
            best = {function, signature};
        }
    }

    return best;
}

csaw::Expect<csaw::RValuePtr> csaw::Builder::CreateCall(const ValuePtr& callee, const LValuePtr& parent, const std::vector<ValuePtr>& args)
{
    if (!callee->GetType()->IsFunctionPointer())
        return Expect<RValuePtr>("Callee is not a function pointer");

    const auto fnty = callee->GetType()->AsPointer()->Base->AsFunction();
    const auto type = Gen(fnty);
    if (!type)
        return Expect<RValuePtr>("Failed to generate type " + fnty->Name + ": " + type.Msg());

    std::vector<llvm::Value*> llvm_args;
    if (parent) llvm_args.push_back(parent->GetPointer());
    size_t i = 0;
    for (; i < fnty->Args.size(); ++i)
    {
        const auto cast = Cast(args[i], fnty->Args[i]);
        if (!cast)
            return Expect<RValuePtr>("Failed to cast: " + cast.Msg());
        llvm_args.push_back(cast.Get()->GetValue());
    }
    for (; i < args.size(); ++i)
        llvm_args.push_back(args[i]->GetValue());

    const auto value = GetBuilder().CreateCall(llvm::cast<llvm::FunctionType>(type.Get()), callee->GetValue(), llvm_args);
    return RValue::Create(this, fnty->Result, value);
}

csaw::Expect<csaw::LValuePtr> csaw::Builder::CreateCtorCall(const ValuePtr& callee, const TypePtr& ctortype, const std::vector<ValuePtr>& args)
{
    if (!callee->GetType()->IsFunctionPointer())
        return Expect<LValuePtr>("Callee is not a function pointer");

    const auto fnty = callee->GetType()->AsPointer()->Base->AsFunction();

    const auto result = Gen(ctortype);
    if (!result)
        return Expect<LValuePtr>("Failed to generate type " + ctortype->Name + ": " + result.Msg());

    std::vector<llvm::Type*> arg_types;
    arg_types.push_back(GetBuilder().getPtrTy());
    for (const auto& arg : fnty->Args)
    {
        const auto gen = Gen(arg);
        if (!gen)
            return Expect<LValuePtr>("Failed to generate type " + arg->Name + ": " + gen.Msg());
        arg_types.push_back(gen.Get());
    }

    llvm::FunctionType* type = llvm::FunctionType::get(result.Get(), arg_types, fnty->IsVararg);

    const auto alloc = LValue::Allocate(this, ctortype);
    if (!alloc)
        return Expect<LValuePtr>("Failed to allocate: " + alloc.Msg());

    const auto& value = alloc.Get();

    std::vector<llvm::Value*> llvm_args;
    llvm_args.push_back(value->GetPointer());
    size_t i = 0;
    for (; i < fnty->Args.size(); ++i)
    {
        const auto cast = Cast(args[i], fnty->Args[i]);
        if (!cast)
            return Expect<LValuePtr>("Failed to cast: " + cast.Msg());
        llvm_args.push_back(cast.Get()->GetValue());
    }
    for (; i < args.size(); ++i)
        llvm_args.push_back(args[i]->GetValue());

    GetBuilder().CreateCall(type, callee->GetValue(), llvm_args);
    return value;
}

csaw::Expect<csaw::RValuePtr> csaw::Builder::FindBestAndCall(const std::string& name, const LValuePtr& parent, const std::vector<ValuePtr>& args)
{
    std::vector<TypePtr> arg_types;
    for (const auto& arg : args)
        arg_types.push_back(arg->GetType());

    const auto [function, signature] = FindBestFunction(name, parent ? parent->GetType() : nullptr, arg_types);
    if (!function)
        return Expect<RValuePtr>("");

    const ValuePtr callee = RValue::Create(this, PointerType::Get(signature.GetFunctionType()), function);

    auto value = CreateCall(callee, parent, args);
    if (!value)
        return Expect<RValuePtr>("Call failed: " + value.Msg());

    return value;
}

csaw::Expect<std::shared_ptr<csaw::LValue>> csaw::Builder::FindBestCtorAndCall(const std::string& name, const std::vector<ValuePtr>& args)
{
    std::vector<TypePtr> arg_types;
    for (const auto& arg : args)
        arg_types.push_back(arg->GetType());

    const auto [function, signature] = FindBestFunction(name, nullptr, arg_types, true);
    if (!function)
        return Expect<LValuePtr>("");

    const ValuePtr callee = RValue::Create(this, PointerType::Get(signature.GetFunctionType()), function);

    auto value = CreateCtorCall(callee, Type::Get(signature.Name), args);
    if (!value)
        return Expect<LValuePtr>("Call failed: " + value.Msg());

    return value;
}

std::map<llvm::Function*, csaw::Signature> csaw::Builder::FindFunctions(const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args, const bool is_ctor) const
{
    std::map<llvm::Function*, Signature> functions;
    for (const auto& [function, signature] : m_Signatures)
    {
        if (signature.IsConstructor() != is_ctor) continue;
        if (signature.Name != name) continue;
        if (signature.Parent != parent) continue;
        if (signature.Args.size() > args.size() || (!signature.IsVarargs && signature.Args.size() != args.size())) continue;
        size_t i = 0;
        for (; i < signature.Args.size(); ++i)
            if (!signature.Args[i]->ParentOf(args[i]))
                break;
        if (i < signature.Args.size()) continue;
        functions[function] = signature;
    }

    return functions;
}

std::pair<llvm::Function*, csaw::Signature> csaw::Builder::FindBestFunction(const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args, const bool is_ctor) const
{
    const auto functions = FindFunctions(name, parent, args, is_ctor);

    int highscore = 0;
    std::pair<llvm::Function*, Signature> best;

    for (const auto& [function, signature] : functions)
    {
        if (const int score = RankFunction(signature, name, parent, args); score >= highscore)
        {
            highscore = score;
            best = {function, signature};
        }
    }

    return best;
}

int csaw::Builder::RankFunction(const Signature& signature, const std::string& name, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    int score = 0;

    for (size_t i = 0; i < signature.Args.size(); ++i)
    {
        if (signature.Args[i] == args[i])
            score += 100;
        else if (signature.Args[i]->ParentOf(args[i]))
            score += 50;
    }

    score += static_cast<int>(args.size() - (args.size() - signature.Args.size()) - 1) * 10;

    return score;
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
