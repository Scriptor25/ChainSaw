#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Def.hpp>
#include <csaw/codegen/Value.hpp>
#include <csaw/lang/Assert.hpp>
#include <csaw/lang/Expr.hpp>

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const ExprPtr &ptr)
{
    if (auto p = std::dynamic_pointer_cast<CallExpr>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<NumExpr>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<ChrExpr>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<StrExpr>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<IdentExpr>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<BinExpr>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<UnExpr>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<IndexExpr>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<MemberExpr>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<VarArgExpr>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<SelExpr>(ptr))
        return CodeGen(builder, *p);

    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const CallExpr &expr)
{
    ValuePtr callee;
    std::string name;
    if (auto ci = std::dynamic_pointer_cast<IdentExpr>(expr.Callee))
        name = ci->Id;
    else if (auto cm = std::dynamic_pointer_cast<MemberExpr>(expr.Callee))
    {
        callee = CodeGen(builder, cm->Object);
        name = cm->Member;
    }
    CHAINSAW_ASSERT(!name.empty(), "Need at least a name to define a function call!");

    std::vector<ValuePtr> args;
    std::vector<TypePtr> argTypes;
    for (const auto &arg: expr.Args)
    {
        const auto value = CodeGen(builder, arg);
        args.push_back(value);
        argTypes.push_back(value->Type);
    }

    const auto function = builder.GetFunction(callee ? callee->Type : nullptr, name, argTypes);

    return builder.CreateCall(function, callee, args);
}

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const NumExpr &expr)
{
    return builder.CreateConstNum(expr.Value);
}

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const ChrExpr &expr)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const StrExpr &expr)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const IdentExpr &expr)
{
    return builder.GetValue(expr.Id);
}

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const BinExpr &expr)
{
    const auto left = CodeGen(builder, expr.Left);
    const auto right = CodeGen(builder, expr.Right);

    if (expr.Operator == "=")
        return builder.CreateStore(left, right);

    if (expr.Operator == "==")
        return builder.CreateCmpEQ(left, right);
    if (expr.Operator == "!=")
        return builder.CreateCmpNE(left, right);
    if (expr.Operator == "<")
        return builder.CreateCmpLT(left, right);
    if (expr.Operator == ">")
        return builder.CreateCmpGT(left, right);
    if (expr.Operator == "<=")
        return builder.CreateCmpLE(left, right);
    if (expr.Operator == ">=")
        return builder.CreateCmpGE(left, right);

    const bool assign = (expr.Operator.back() == '=');
    const auto op = (assign ? expr.Operator.substr(0, expr.Operator.size() - 1) : expr.Operator);

    ValuePtr value;
    if (op == "&")
        value = builder.CreateAnd(left, right);
    if (op == "&&")
        value = builder.CreateLAnd(left, right);
    if (op == "|")
        value = builder.CreateOr(left, right);
    if (op == "||")
        value = builder.CreateLOr(left, right);
    if (op == "^")
        value = builder.CreateXOr(left, right);
    if (op == "<<")
        value = builder.CreateShL(left, right);
    if (op == ">>")
        value = builder.CreateShR(left, right);
    if (op == "+")
        value = builder.CreateAdd(left, right);
    if (op == "-")
        value = builder.CreateSub(left, right);
    if (op == "*")
        value = builder.CreateMul(left, right);
    if (op == "/")
        value = builder.CreateDiv(left, right);
    if (op == "%")
        value = builder.CreateRem(left, right);

    if (!value)
        throw std::runtime_error("Not yet implemented");

    if (assign)
        return builder.CreateStore(left, value);

    return value;
}

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const UnExpr &expr)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const IndexExpr &expr)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const MemberExpr &expr)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const VarArgExpr &expr)
{
    throw std::runtime_error("Not yet implemented");
}

csaw::ValuePtr csaw::CodeGen(csaw::Builder &builder, const SelExpr &expr)
{
    throw std::runtime_error("Not yet implemented");
}
