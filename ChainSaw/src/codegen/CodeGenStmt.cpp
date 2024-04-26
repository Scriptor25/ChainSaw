#include <csaw/codegen/Block.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Def.hpp>
#include <csaw/codegen/Function.hpp>
#include <csaw/codegen/Value.hpp>
#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Stmt.hpp>

void csaw::CodeGen(Builder &builder, const StmtPtr &ptr)
{
    if (auto p = std::dynamic_pointer_cast<FunStmt>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<RetStmt>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<EnclosedStmt>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<ForStmt>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<VarStmt>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<WhileStmt>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<IfStmt>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<ThingStmt>(ptr))
        return CodeGen(builder, *p);
    if (auto p = std::dynamic_pointer_cast<AliasStmt>(ptr))
        return CodeGen(builder, *p);

    if (auto p = std::dynamic_pointer_cast<Expr>(ptr))
    {
        CodeGen(builder, p);
        return;
    }

    throw std::runtime_error("Not yet implemented");
}

void csaw::CodeGen(Builder &builder, const FunStmt &stmt)
{
    std::vector<TypePtr> argTypes;
    argTypes.reserve(stmt.Args.size());
    for (const auto &arg: stmt.Args)
        argTypes.push_back(arg.second);

    const auto funTy = FunType::Get(stmt.Callee, argTypes, !stmt.VarArg.empty(), stmt.Result);
    const auto function = builder.CreateFunction(stmt.Name, funTy, stmt.Constructor);

    for (size_t i = 0; i < stmt.Args.size(); ++i)
        function->Args[i]->Name = stmt.Args[i].first;

    if (!stmt.Body)
        return;

    if (function->Implemented())
        throw std::runtime_error("Function already implemented!");

    builder.ClearValues();
    for (const auto &arg: function->Args)
        builder.SetValue(arg->Name, arg);
    if (function->Me)
        builder.SetValue("me", function->Me);

    const auto entryBlock = Block::Create(function);
    function->SetEntry(entryBlock);
    builder.SetInsertPoint(entryBlock);

    CodeGen(builder, *stmt.Body);

    builder.SetInsertPoint(nullptr);
    builder.ClearValues();
}

void csaw::CodeGen(Builder &builder, const RetStmt &stmt)
{
    const auto value = stmt.Value ? CodeGen(builder, stmt.Value) : nullptr;
    builder.CreateRet(value);
}

void csaw::CodeGen(Builder &builder, const EnclosedStmt &stmt)
{
    for (const auto &ptr: stmt.Content)
        CodeGen(builder, ptr);
}

void csaw::CodeGen(Builder &builder, const ForStmt &stmt)
{
    throw std::runtime_error("Not yet implemented");
}

void csaw::CodeGen(Builder &builder, const VarStmt &stmt)
{
    throw std::runtime_error("Not yet implemented");
}

void csaw::CodeGen(Builder &builder, const WhileStmt &stmt)
{
    throw std::runtime_error("Not yet implemented");
}

void csaw::CodeGen(Builder &builder, const IfStmt &stmt)
{
    const auto function = builder.GetInsertPoint()->Parent;

    const auto thenBlock = Block::Create(function);
    const auto elseBlock = Block::Create(function);
    const auto endBlock = (stmt.False ? Block::Create(function) : elseBlock);

    const auto condition = CodeGen(builder, stmt.Condition);
    builder.CreateBranch(condition, {thenBlock, elseBlock});

    builder.SetInsertPoint(thenBlock);
    CodeGen(builder, stmt.True);
    builder.CreateBranch(nullptr, {endBlock});

    builder.SetInsertPoint(elseBlock);
    if (stmt.False)
    {
        CodeGen(builder, stmt.False);
        builder.CreateBranch(nullptr, {endBlock});
    }

    builder.SetInsertPoint(endBlock);
}

void csaw::CodeGen(Builder &builder, const ThingStmt &stmt)
{
    const auto thingTy = ThingType::Get(stmt.Name, stmt.Group);

    if (stmt.Elements.empty())
        return;

    if (thingTy->Implemented())
        throw std::runtime_error("Thing already implemented!");

    thingTy->Elements = stmt.Elements;
}

void csaw::CodeGen(Builder &builder, const AliasStmt &stmt)
{
    throw std::runtime_error("Not yet implemented");
}
