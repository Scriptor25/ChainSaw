#include <iostream>
#include <csaw/Builder.hpp>
#include <llvm/IR/Verifier.h>
#include <llvm/Linker/Linker.h>
#include <llvm/Transforms/Utils/ModuleUtils.h>

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
