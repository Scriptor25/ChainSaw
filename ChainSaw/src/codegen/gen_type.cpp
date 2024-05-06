#include <csaw/codegen/Builder.hpp>

llvm::Type* csaw::Builder::Gen(const TypePtr& ptr)
{
    if (!ptr)
        return m_Builder->getVoidTy();

    if (const auto ptrty = std::dynamic_pointer_cast<PointerType>(ptr))
        return m_Builder->getPtrTy();

    if (const auto structty = llvm::StructType::getTypeByName(*m_Context, ptr->Name))
        return structty;

    if (ptr->Name == "int1") return m_Builder->getInt1Ty();
    if (ptr->Name == "int8") return m_Builder->getInt8Ty();
    if (ptr->Name == "int16") return m_Builder->getInt16Ty();
    if (ptr->Name == "int32") return m_Builder->getInt32Ty();
    if (ptr->Name == "int64") return m_Builder->getInt64Ty();
    if (ptr->Name == "int128") return m_Builder->getInt128Ty();

    if (ptr->Name == "flt16") return m_Builder->getHalfTy();
    if (ptr->Name == "flt32") return m_Builder->getFloatTy();
    if (ptr->Name == "flt64") return m_Builder->getDoubleTy();

    throw std::runtime_error("not yet implemented");
}
