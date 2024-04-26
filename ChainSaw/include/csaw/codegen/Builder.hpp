#pragma once

#include <csaw/codegen/Def.hpp>
#include <csaw/Type.hpp>

#include <map>
#include <memory>
#include <vector>

namespace csaw
{
    class Builder
    {
    public:
        Builder();

        FunctionPtr CreateFunction(const std::string &name, const FunTypePtr &type, bool constructor);

        FunctionPtr GetFunction(const std::string &name, const FunTypePtr &type);

        FunctionPtr GetFunction(const TypePtr &callee, const std::string &name, const std::vector<TypePtr> &args);

        void SetInsertPoint(const BlockPtr &ptr);

        BlockPtr GetInsertPoint();

        void ClearValues();

        void SetValue(const std::string &name, const ValuePtr &value);

        ValuePtr GetValue(const std::string &name);

        ValuePtr CreateStore(const ValuePtr &dst, const ValuePtr &src);

        ValuePtr CreateCmpEQ(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateCmpNE(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateCmpLT(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateCmpGT(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateCmpLE(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateCmpGE(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateAnd(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateLAnd(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateOr(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateLOr(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateXOr(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateShL(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateShR(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateAdd(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateSub(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateMul(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateDiv(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateRem(const ValuePtr &left, const ValuePtr &right);

        ValuePtr CreateCall(const FunctionPtr &function, const ValuePtr &callee, const std::vector<ValuePtr> &args);

        ValuePtr CreateConstNum(double x);

        void CreateBranch(const ValuePtr &condition, const std::vector<BlockPtr> &blocks);

        void CreateRet(const ValuePtr &value);

    private:
        std::map<std::pair<std::string, FunTypePtr>, FunctionPtr> m_Functions;
        std::map<std::string, ValuePtr> m_Values;
        BlockPtr m_InsertPoint;
    };
}
