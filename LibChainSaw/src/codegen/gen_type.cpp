#include <csaw/codegen/Builder.hpp>

llvm::Type* csaw::Builder::Gen(const TypePtr& type) const
{
    if (!type)
        return nullptr;

    if (type->IsPointer())
        return m_Builder->getPtrTy();

    if (type->IsArray())
    {
        const auto arrayty = type->AsArray();
        return llvm::ArrayType::get(Gen(arrayty->Base), arrayty->Size);
    }

    if (type->IsStruct())
        return llvm::StructType::getTypeByName(*m_Context, type->Name);

    if (type->IsFunction())
    {
        const auto fty = type->AsFunction();
        const auto result = Gen(fty->Result);
        std::vector<llvm::Type*> args;
        for (const auto& arg : fty->Args)
            args.push_back(Gen(arg));
        return llvm::FunctionType::get(result, args, fty->IsVararg);
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

    return nullptr;
}
