#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>

llvm::Type* csaw::Builder::Gen(const TypePtr& type) const
{
    if (!type)
        CSAW_MESSAGE_NONE(true, "type must not be null");

    if (type->IsPointer() || type->IsFunction())
        return m_Builder->getPtrTy();

    if (type->IsArray())
    {
        const auto arrayty = type->AsArray();
        return llvm::ArrayType::get(Gen(arrayty->Base), arrayty->Size);
    }

    if (type->IsStruct())
    {
        if (const auto structty = llvm::StructType::getTypeByName(*m_Context, type->Name))
            return structty;
        CSAW_MESSAGE_NONE(true, type->Name + " is a struct type, but there is no llvm struct type defined with this name");
    }

    if (type->Name == "void") return m_Builder->getVoidTy();
    if (type->Name == "int1") return m_Builder->getInt1Ty();
    if (type->Name == "int8") return m_Builder->getInt8Ty();
    if (type->Name == "int16") return m_Builder->getInt16Ty();
    if (type->Name == "int32") return m_Builder->getInt32Ty();
    if (type->Name == "int64") return m_Builder->getInt64Ty();
    if (type->Name == "int128") return m_Builder->getInt128Ty();
    if (type->Name == "flt16") return m_Builder->getHalfTy();
    if (type->Name == "flt32") return m_Builder->getFloatTy();
    if (type->Name == "flt64") return m_Builder->getDoubleTy();

    CSAW_MESSAGE_NONE(true, "no type with name '" + type->Name + "' defined");
}
