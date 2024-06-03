#include <csaw/Builder.hpp>
#include <csaw/Type.hpp>

csaw::Expect<llvm::Type*> csaw::Builder::Gen(const TypePtr& type) const
{
    if (!type)
        return Expect<llvm::Type*>("Type is null");

    if (type->IsPointer())
        return GetBuilder().getPtrTy();

    if (type->IsArray())
    {
        const auto arrayty = type->AsArray();
        const auto base = Gen(arrayty->Base);
        if (!base)
            return Expect<llvm::Type*>("Failed to generate base type of array type " + type->Name + ": " + base.Msg());

        return llvm::ArrayType::get(base.Get(), arrayty->Size);
    }

    if (type->IsStruct())
        if (const auto structty = llvm::StructType::getTypeByName(GetContext(), type->Name))
            return structty;

    if (type->IsFunction())
    {
        const auto fty = type->AsFunction();

        const auto result = Gen(fty->Result);
        if (!result)
            return Expect<llvm::Type*>("Function result of " + type->Name + " is null: " + result.Msg());

        std::vector<llvm::Type*> args;
        if (fty->Parent)
        {
            const auto parent = Gen(PointerType::Get(fty->Parent));
            if (!parent)
                return Expect<llvm::Type*>("Function parent of " + type->Name + " is null: " + parent.Msg());

            args.push_back(parent.Get());
        }
        for (const auto& arg : fty->Args)
        {
            const auto argtype = Gen(arg);
            if (!argtype)
                return Expect<llvm::Type*>("Function argument #" + std::to_string(args.size()) + " of " + type->Name + " is null: " + argtype.Msg());
            args.push_back(argtype.Get());
        }

        return llvm::FunctionType::get(result.Get(), args, fty->IsVararg);
    }

    if (type->Name == "void") return GetBuilder().getVoidTy();
    if (type->Name == "int1") return GetBuilder().getInt1Ty();
    if (type->Name == "int8") return GetBuilder().getInt8Ty();
    if (type->Name == "int16") return GetBuilder().getInt16Ty();
    if (type->Name == "int32") return GetBuilder().getInt32Ty();
    if (type->Name == "int64") return GetBuilder().getInt64Ty();
    if (type->Name == "int128") return GetBuilder().getInt128Ty();
    if (type->Name == "flt16") return GetBuilder().getHalfTy();
    if (type->Name == "flt32") return GetBuilder().getFloatTy();
    if (type->Name == "flt64") return GetBuilder().getDoubleTy();

    return Expect<llvm::Type*>("Type " + type->Name + " does not exist");
}
