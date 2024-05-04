#include <csaw/codegen/Builder.hpp>

csaw::Builder::Builder(const std::string& moduleName)
{
    m_Context = std::make_unique<llvm::LLVMContext>();
    m_Builder = std::make_unique<llvm::IRBuilder<>>(*m_Context);
    m_Module = std::make_unique<llvm::Module>(moduleName, *m_Context);
}

bool csaw::Builder::IsGlobal() const
{
    return !m_Builder->GetInsertBlock();
}
