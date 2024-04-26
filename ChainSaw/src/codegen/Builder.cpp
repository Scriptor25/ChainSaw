#include <csaw/codegen/Block.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Function.hpp>
#include <csaw/codegen/Instruction.hpp>
#include <csaw/codegen/Value.hpp>
#include <csaw/lang/Assert.hpp>
#include <csaw/Type.hpp>

csaw::Builder::Builder()
= default;

csaw::FunctionPtr csaw::Builder::CreateFunction(const std::string &name, const FunTypePtr &type, bool constructor)
{
    auto &function = m_Functions[{name, type}];
    if (!function) function = std::make_shared<Function>(name, type, constructor);
    return function;
}

csaw::FunctionPtr csaw::Builder::GetFunction(const std::string &name, const csaw::FunTypePtr &type)
{
    return m_Functions[{name, type}];
}

csaw::FunctionPtr
csaw::Builder::GetFunction(const TypePtr &callee, const std::string &name, const std::vector<TypePtr> &args)
{
    for (const auto &entry: m_Functions)
    {
        if (entry.first.first != name)
            continue;

        const auto type = entry.first.second;

        if (type->Callee != callee)
            continue;
        if (type->Args.size() > args.size() || (type->Args.size() != args.size() && !type->VarArg))
            continue;

        size_t i;
        for (i = 0; i < type->Args.size(); ++i)
            if (type->Args[i] != args[i])
                break;
        if (i < type->Args.size())
            continue;

        return entry.second;
    }
    return nullptr;
}

void csaw::Builder::SetInsertPoint(const csaw::BlockPtr &ptr)
{
    m_InsertPoint = ptr;
}

csaw::BlockPtr csaw::Builder::GetInsertPoint()
{
    return m_InsertPoint;
}

void csaw::Builder::ClearValues()
{
    m_Values.clear();
}

void csaw::Builder::SetValue(const std::string &name, const ValuePtr &value)
{
    auto &ref = m_Values[name];
    if (ref)
        throw std::runtime_error("Cannot reassign named value!");
    ref = value;
}

csaw::ValuePtr csaw::Builder::GetValue(const std::string &name)
{
    return m_Values[name];
}

csaw::ValuePtr csaw::Builder::CreateStore(const ValuePtr &dst, const ValuePtr &src)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateCmpEQ(const ValuePtr &left, const ValuePtr &right)
{
    const auto numTy = Type::Get("num");
    CHAINSAW_ASSERT(left->Type == numTy && left->Type == right->Type,
                    "Left and right must both be of type number!");

    const auto result = std::make_shared<Value>(std::string(), numTy);
    const auto inst = std::make_shared<InstSimple>(result, "cmp_eq", std::vector<ValuePtr>({left, right}));
    m_InsertPoint->Insert(inst);

    return result;
}

csaw::ValuePtr csaw::Builder::CreateCmpNE(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateCmpLT(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateCmpGT(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateCmpLE(const ValuePtr &left, const ValuePtr &right)
{
    const auto numTy = Type::Get("num");
    CHAINSAW_ASSERT(left->Type == numTy && left->Type == right->Type,
                    "Left and right must both be of type number!");

    const auto result = std::make_shared<Value>(std::string(), numTy);
    const auto inst = std::make_shared<InstSimple>(result, "cmp_le", std::vector<ValuePtr>({left, right}));
    m_InsertPoint->Insert(inst);

    return result;
}

csaw::ValuePtr csaw::Builder::CreateCmpGE(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateAnd(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateLAnd(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateOr(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateLOr(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateXOr(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateShL(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateShR(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateAdd(const ValuePtr &left, const ValuePtr &right)
{
    const auto numTy = Type::Get("num");
    CHAINSAW_ASSERT(left->Type == numTy && left->Type == right->Type,
                    "Left and right must both be of type number!");

    const auto result = std::make_shared<Value>(std::string(), numTy);
    const auto inst = std::make_shared<InstSimple>(result, "add", std::vector<ValuePtr>({left, right}));
    m_InsertPoint->Insert(inst);

    return result;
}

csaw::ValuePtr csaw::Builder::CreateSub(const ValuePtr &left, const ValuePtr &right)
{
    const auto numTy = Type::Get("num");
    CHAINSAW_ASSERT(left->Type == numTy && left->Type == right->Type,
                    "Left and right must both be of type number!");

    const auto result = std::make_shared<Value>(std::string(), numTy);
    const auto inst = std::make_shared<InstSimple>(result, "sub", std::vector<ValuePtr>({left, right}));
    m_InsertPoint->Insert(inst);

    return result;
}

csaw::ValuePtr csaw::Builder::CreateMul(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateDiv(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::Builder::CreateRem(const ValuePtr &left, const ValuePtr &right)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr
csaw::Builder::CreateCall(const FunctionPtr &function, const ValuePtr &callee, const std::vector<ValuePtr> &args)
{
    const auto result = std::make_shared<Value>(std::string(), function->Type->Result);
    const auto inst = std::make_shared<InstCall>(result, function, callee, args);
    m_InsertPoint->Insert(inst);

    return result;
}

csaw::ValuePtr csaw::Builder::CreateConstNum(const double x)
{
    const auto type = Type::Get("num");
    return std::make_shared<NumValue>(std::string(), type, x);
}

void csaw::Builder::CreateBranch(const ValuePtr &condition, const std::vector<BlockPtr> &blocks)
{
    const auto inst = std::make_shared<InstBranch>(condition, blocks);
    m_InsertPoint->Insert(inst);
}

void csaw::Builder::CreateRet(const csaw::ValuePtr &value)
{
    const auto inst = std::make_shared<InstRet>(value);
    m_InsertPoint->Insert(inst);
}
