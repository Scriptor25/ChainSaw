#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>

llvm::Type* csaw::Builder::Gen(const TypePtr& ptr) const
{
    if (!ptr)
        CSAW_MESSAGE_NONE(true, "type must not be null");

    if (ptr->IsPointer())
        return m_Builder->getPtrTy();

    if (ptr->IsArray())
    {
        const auto type = ptr->AsArray();
        return llvm::ArrayType::get(Gen(type->Base), type->Size);
    }

    if (ptr->IsStruct())
    {
        if (const auto type = llvm::StructType::getTypeByName(*m_Context, ptr->Name))
            return type;
        CSAW_MESSAGE_NONE(true, ptr->Name + " is a struct type, but there is no llvm struct type defined with this name");
    }

    if (ptr->Name == "void") return m_Builder->getVoidTy();
    if (ptr->Name == "int1") return m_Builder->getInt1Ty();
    if (ptr->Name == "int8") return m_Builder->getInt8Ty();
    if (ptr->Name == "int16") return m_Builder->getInt16Ty();
    if (ptr->Name == "int32") return m_Builder->getInt32Ty();
    if (ptr->Name == "int64") return m_Builder->getInt64Ty();
    if (ptr->Name == "int128") return m_Builder->getInt128Ty();
    if (ptr->Name == "flt16") return m_Builder->getHalfTy();
    if (ptr->Name == "flt32") return m_Builder->getFloatTy();
    if (ptr->Name == "flt64") return m_Builder->getDoubleTy();

    CSAW_MESSAGE_NONE(true, "no type with name '" + ptr->Name + "' defined");
}
