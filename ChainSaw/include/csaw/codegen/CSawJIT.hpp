#pragma once

#include <memory>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/Core.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/IR/DataLayout.h>

namespace csaw
{
    class CSawJIT
    {
    public:
        static llvm::Expected<std::unique_ptr<CSawJIT>> Create();

        CSawJIT(std::unique_ptr<llvm::orc::ExecutionSession> session, const llvm::orc::JITTargetMachineBuilder& builder, const llvm::DataLayout& layout);

        const llvm::DataLayout& GetDataLayout() const;

        llvm::Error AddModule(std::unique_ptr<llvm::Module>& module);
        llvm::Expected<llvm::orc::ExecutorSymbolDef> Lookup(const std::string& name);

    private:
        std::unique_ptr<llvm::orc::ExecutionSession> m_Session;
        llvm::orc::RTDyldObjectLinkingLayer m_LinkerLayer;
        llvm::orc::IRCompileLayer m_CompileLayer;
        llvm::DataLayout m_Layout;
        llvm::orc::MangleAndInterner m_Mangle;
        llvm::orc::ThreadSafeContext m_Context;
        llvm::orc::JITDylib& m_MainDy;
    };
}
