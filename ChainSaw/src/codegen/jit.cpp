#include <csaw/codegen/CSawJIT.hpp>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>

llvm::Expected<std::unique_ptr<csaw::CSawJIT>> csaw::CSawJIT::Create()
{
    auto control = llvm::orc::SelfExecutorProcessControl::Create();
    if (!control)
        return control.takeError();

    auto session = std::make_unique<llvm::orc::ExecutionSession>(std::move(*control));

    auto builder = llvm::orc::JITTargetMachineBuilder(session->getExecutorProcessControl().getTargetTriple());

    auto layout = builder.getDefaultDataLayoutForTarget();
    if (!layout)
        return layout.takeError();

    return std::make_unique<CSawJIT>(std::move(session), std::move(builder), std::move(*layout));
}

csaw::CSawJIT::CSawJIT(std::unique_ptr<llvm::orc::ExecutionSession> session, const llvm::orc::JITTargetMachineBuilder& builder, const llvm::DataLayout& layout)
    : m_Session(std::move(session)),
      m_LinkerLayer(*m_Session, []() { return std::make_unique<llvm::SectionMemoryManager>(); }),
      m_CompileLayer(*m_Session, m_LinkerLayer, std::make_unique<llvm::orc::ConcurrentIRCompiler>(builder)),
      m_Layout(layout),
      m_Mangle(*m_Session, m_Layout),
      m_Context(std::make_unique<llvm::LLVMContext>()),
      m_MainDy(m_Session->createBareJITDylib("<main>"))
{
    m_MainDy.addGenerator(llvm::cantFail(llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(m_Layout.getGlobalPrefix())));
}

const llvm::DataLayout& csaw::CSawJIT::GetDataLayout() const
{
    return m_Layout;
}

llvm::Error csaw::CSawJIT::AddModule(std::unique_ptr<llvm::Module>& module)
{
    return m_CompileLayer.add(m_MainDy, llvm::orc::ThreadSafeModule(std::move(module), m_Context));
}

llvm::Expected<llvm::orc::ExecutorSymbolDef> csaw::CSawJIT::Lookup(const std::string& name)
{
    return m_Session->lookup({&m_MainDy}, m_Mangle(llvm::StringRef(name)));
}
