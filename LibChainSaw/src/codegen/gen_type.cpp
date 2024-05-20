#include <csaw/codegen/Builder.hpp>

csaw::Expect<llvm::Type*> csaw::Builder::Gen(const TypePtr& type) const
{
    if (!type)
        return Expect<llvm::Type*>("Type is null");

    if (type->IsPointer())
        return m_Builder->getPtrTy();

    if (type->IsArray())
    {
        const auto arrayty = type->AsArray();
        const auto base = Gen(arrayty->Base);
        if (!base)
            return Expect<llvm::Type*>("Array base of " + type->Name + " is null: " + base.Msg());

        if (const auto arrty = llvm::ArrayType::get(base.Get(), arrayty->Size))
            return arrty;
    }

    if (type->IsStruct())
    {
        if (const auto structty = llvm::StructType::getTypeByName(*m_Context, type->Name))
            return structty;
    }

    if (type->IsFunction())
    {
        const auto fty = type->AsFunction();
        const auto result = Gen(fty->Result);
        if (!result)
            return Expect<llvm::Type*>("Function result of " + type->Name + " is null: " + result.Msg());

        std::vector<llvm::Type*> args;
        for (const auto& arg : fty->Args)
        {
            const auto argtype = Gen(arg);
            if (!argtype)
                return Expect<llvm::Type*>("Function argument #" + std::to_string(args.size()) + " of " + type->Name + " is null: " + argtype.Msg());
            args.push_back(argtype.Get());
        }
        return llvm::FunctionType::get(result.Get(), args, fty->IsVararg);
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

    return Expect<llvm::Type*>("Type " + type->Name + " has no llvm equivalent");
}
