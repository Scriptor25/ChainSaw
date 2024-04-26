#pragma once

#include <csaw/lang/Def.hpp>

namespace csaw
{
    class Builder;

    struct Function;
    typedef std::shared_ptr<Function> FunctionPtr;

    struct Block;
    typedef std::shared_ptr<Block> BlockPtr;

    struct Value;
    typedef std::shared_ptr<Value> ValuePtr;

    struct NumValue;

    struct Arg;
    typedef std::shared_ptr<Arg> ArgPtr;

    struct Instruction;
    typedef std::shared_ptr<Instruction> InstructionPtr;

    struct InstSimple;
    struct InstCall;

    struct InstBranch;
    struct InstRet;

    void CodeGen(Builder &builder, const StmtPtr &ptr);

    void CodeGen(Builder &builder, const FunStmt &stmt);

    void CodeGen(Builder &builder, const RetStmt &stmt);

    void CodeGen(Builder &builder, const EnclosedStmt &stmt);

    void CodeGen(Builder &builder, const ForStmt &stmt);

    void CodeGen(Builder &builder, const VarStmt &stmt);

    void CodeGen(Builder &builder, const WhileStmt &stmt);

    void CodeGen(Builder &builder, const IfStmt &stmt);

    void CodeGen(Builder &builder, const ThingStmt &stmt);

    void CodeGen(Builder &builder, const AliasStmt &stmt);

    ValuePtr CodeGen(Builder &builder, const ExprPtr &ptr);

    ValuePtr CodeGen(Builder &builder, const CallExpr &expr);

    ValuePtr CodeGen(Builder &builder, const NumExpr &expr);

    ValuePtr CodeGen(Builder &builder, const ChrExpr &expr);

    ValuePtr CodeGen(Builder &builder, const StrExpr &expr);

    ValuePtr CodeGen(Builder &builder, const IdentExpr &expr);

    ValuePtr CodeGen(Builder &builder, const BinExpr &expr);

    ValuePtr CodeGen(Builder &builder, const UnExpr &expr);

    ValuePtr CodeGen(Builder &builder, const IndexExpr &expr);

    ValuePtr CodeGen(Builder &builder, const MemberExpr &expr);

    ValuePtr CodeGen(Builder &builder, const VarArgExpr &expr);

    ValuePtr CodeGen(Builder &builder, const SelExpr &expr);
}
