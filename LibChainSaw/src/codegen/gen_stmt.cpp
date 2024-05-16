#include <iostream>
#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Signature.hpp>
#include <csaw/codegen/Value.hpp>
#include <csaw/lang/Expr.hpp>
#include <csaw/lang/Stmt.hpp>
#include <llvm/IR/Verifier.h>

void csaw::Builder::Gen(const StatementPtr& ptr)
{
    try
    {
        if (!ptr)
            CSAW_MESSAGE_NONE(true, "statement must not be null");

        if (const auto p = std::dynamic_pointer_cast<ScopeStatement>(ptr))
            return Gen(*p);
        if (const auto p = std::dynamic_pointer_cast<ForStatement>(ptr))
            return Gen(*p);
        if (const auto p = std::dynamic_pointer_cast<FunctionStatement>(ptr))
            return Gen(*p);
        if (const auto p = std::dynamic_pointer_cast<IfStatement>(ptr))
            return Gen(*p);
        if (const auto p = std::dynamic_pointer_cast<RetStatement>(ptr))
            return Gen(*p);
        if (const auto p = std::dynamic_pointer_cast<DefStatement>(ptr))
            return Gen(*p);
        if (const auto p = std::dynamic_pointer_cast<VariableStatement>(ptr))
            return Gen(*p);
        if (const auto p = std::dynamic_pointer_cast<WhileStatement>(ptr))
            return Gen(*p);

        if (const auto p = std::dynamic_pointer_cast<Expression>(ptr))
        {
            (void)Gen(p);
            return;
        }

        CSAW_MESSAGE_STMT(true, *ptr, "code generation for statement is not implemented");
    }
    catch (const ChainSawMessage& error)
    {
        const auto file = error.SourceFile.empty() ? (ptr ? ptr->Filename : "<none>") : error.SourceFile;
        const auto line = error.SourceLine == 0 ? (ptr ? ptr->Line : 0) : error.SourceLine;
        std::cerr << file << "(" << line << "): " << error.Message << std::endl;
        if (!error.CanRecover) throw;
    }
}

void csaw::Builder::Gen(const ScopeStatement& statement)
{
    for (const auto& ptr : statement.Body)
        Gen(ptr);
}

void csaw::Builder::Gen(const ForStatement& statement)
{
    const auto parent = m_Builder->GetInsertBlock()->getParent();
    const auto hdr_block = llvm::BasicBlock::Create(*m_Context, "hdr", parent);
    const auto loop_block = llvm::BasicBlock::Create(*m_Context, "loop", parent);
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end", parent);

    if (statement.Pre) Gen(statement.Pre);
    m_Builder->CreateBr(hdr_block);
    m_Builder->SetInsertPoint(hdr_block);
    const auto condition = statement.Condition ? Gen(statement.Condition) : RValue::Create(Type::GetInt1(), m_Builder->getInt1(true));
    m_Builder->CreateCondBr(condition->GetValue(), loop_block, end_block);
    m_Builder->SetInsertPoint(loop_block);
    if (statement.Body) Gen(statement.Body);
    if (statement.Loop) Gen(statement.Loop);
    m_Builder->CreateBr(hdr_block);
    m_Builder->SetInsertPoint(end_block);
}

void csaw::Builder::Gen(const FunctionStatement& statement)
{
    const bool has_ptr = statement.IsConstructor() || statement.Parent;

    if (statement.IsConstructor())
        Type::Get(statement.Name);

    std::vector<TypePtr> args(statement.Args.size());
    std::vector<llvm::Type*> arg_types(statement.Args.size());
    for (size_t i = 0; i < args.size(); ++i) arg_types[i] = Gen(args[i] = statement.Args[i].second);
    if (has_ptr) arg_types.insert(arg_types.begin(), m_Builder->getPtrTy());

    auto [signature, function] = FindFunction(statement.Name, statement.Parent, args);

    if (!function)
    {
        signature.Result = statement.Result;
        signature.IsVarargs = statement.IsVarArgs;
        signature.IsC = statement.Name == "main" || std::ranges::find(statement.Mods, "c") != statement.Mods.end();

        const auto result_type = signature.IsConstructor() ? Gen(Type::GetVoid()) : Gen(statement.Result);
        const auto function_type = llvm::FunctionType::get(result_type, arg_types, statement.IsVarArgs);
        function = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, signature.Mangle(), *m_Module);
    }

    if (!statement.Body)
        return;

    if (!function->empty())
        CSAW_MESSAGE_STMT(true, statement, "function is already implemented");

    const auto entry_block = llvm::BasicBlock::Create(*m_Context, "entry", function);
    const auto backup_block = m_Builder->GetInsertBlock();
    m_Builder->SetInsertPoint(entry_block);

    m_Values.clear();
    int i = has_ptr ? -1 : 0;
    for (auto& arg : function->args())
    {
        const auto name = i < 0 ? "me" : statement.Args[i].first;
        arg.setName(name);

        if (i < 0)
        {
            const auto type = signature.IsConstructor() ? Type::Get(statement.Name) : statement.Parent;
            m_Values["me"] = LValue::AllocateAndStore(this, PointerType::Get(type), &arg);
        }
        else
        {
            m_Values[name] = LValue::AllocateAndStore(this, statement.Args[i].second, &arg);
        }
        ++i;
    }

    if (const auto scope_statement = std::dynamic_pointer_cast<ScopeStatement>(statement.Body))
    {
        Gen(scope_statement);

        if (function->getFunctionType()->getReturnType()->isVoidTy())
            for (const auto& block : *function)
                if (!block.getTerminator())
                    m_Builder->CreateRetVoid();
    }
    else if (const auto expression = std::dynamic_pointer_cast<Expression>(statement.Body))
    {
        const auto result = Gen(expression);
        if (function->getFunctionType()->getReturnType()->isVoidTy())
            m_Builder->CreateRetVoid();
        else m_Builder->CreateRet(result->GetValue());
    }
    else
    {
        Gen(statement.Body);
        m_Builder->CreateRetVoid();
    }

    m_Builder->SetInsertPoint(backup_block);

    if (verifyFunction(*function, &llvm::errs()))
    {
        function->viewCFG();
        function->eraseFromParent();
        CSAW_MESSAGE_STMT(true, statement, "failed to verify function");
    }

    m_FPM->run(*function, *m_FAM);
}

void csaw::Builder::Gen(const IfStatement& statement)
{
    const auto parent = m_Builder->GetInsertBlock()->getParent();
    auto true_block = llvm::BasicBlock::Create(*m_Context, "true", parent);
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end");
    auto false_block = statement.False ? llvm::BasicBlock::Create(*m_Context, "false") : end_block;

    const auto condition = Gen(statement.Condition);
    m_Builder->CreateCondBr(condition->GetValue(), true_block, false_block);

    bool need_end = false;

    m_Builder->SetInsertPoint(true_block);
    Gen(statement.True);
    true_block = m_Builder->GetInsertBlock();
    if (!true_block->getTerminator())
    {
        m_Builder->CreateBr(end_block);
        need_end = true;
    }

    parent->insert(parent->end(), false_block);
    m_Builder->SetInsertPoint(false_block);
    if (statement.False)
    {
        Gen(statement.False);
        false_block = m_Builder->GetInsertBlock();
        if (!false_block->getTerminator())
        {
            m_Builder->CreateBr(end_block);
            need_end = true;
        }
    }
    else need_end = false;

    if (need_end)
    {
        parent->insert(parent->end(), end_block);
        m_Builder->SetInsertPoint(end_block);
    }
}

void csaw::Builder::Gen(const RetStatement& statement)
{
    if (!statement.Value)
    {
        m_Builder->CreateRetVoid();
        return;
    }

    const auto value = Gen(statement.Value);
    m_Builder->CreateRet(value->GetValue());
}

void csaw::Builder::Gen(const DefStatement& statement) const
{
    if (statement.Origin)
    {
        Type::Alias(statement.Name, statement.Origin);
        return;
    }

    if (statement.Elements.empty())
    {
        if (!llvm::StructType::getTypeByName(*m_Context, statement.Name))
            llvm::StructType::create(*m_Context, statement.Name);
        return;
    }

    const auto type = llvm::StructType::getTypeByName(*m_Context, statement.Name);
    if (type && !type->isEmptyTy())
        CSAW_MESSAGE_STMT(true, statement, "struct is already implemented");

    std::vector<llvm::Type*> elements(statement.Elements.size());
    for (size_t i = 0; i < elements.size(); ++i)
        elements[i] = Gen(statement.Elements[i].second);

    if (!type)
        llvm::StructType::create(*m_Context, elements, statement.Name);
    else
        type->setBody(elements);
}

void csaw::Builder::Gen(const VariableStatement& statement)
{
    const auto type = Gen(statement.Type);

    ValuePtr initializer;
    if (statement.Value) initializer = Gen(statement.Value);
    else if (const auto [signature, function] = FindFunction(statement.Type->Name, nullptr, {}); function && signature.IsConstructor())
    {
        const auto linitializer = LValue::Allocate(this, Type::Get(signature.Name));
        initializer = linitializer;
        m_Builder->CreateCall(function->getFunctionType(), function, {linitializer->GetPointer()});
    }

    if (initializer)
        initializer = Cast(initializer, statement.Type);

    if (m_Builder->GetInsertBlock() == &m_GlobalParent->getEntryBlock())
    {
        llvm::Constant* global_initializer = nullptr;
        if (initializer)
            global_initializer = llvm::dyn_cast<llvm::Constant>(initializer->GetValue());

        const bool is_initialized = global_initializer;
        if (!is_initialized)
            global_initializer = llvm::Constant::getNullValue(type);

        const auto pointer = new llvm::GlobalVariable(*m_Module, type, false, llvm::GlobalValue::InternalLinkage, global_initializer, statement.Name);
        const auto ref = LValue::Direct(this, statement.Type, pointer);

        if (!is_initialized && initializer)
            ref->StoreValue(initializer->GetValue());

        m_GlobalValues[statement.Name] = ref;
        return;
    }

    m_Values[statement.Name] = LValue::AllocateAndStore(this, statement.Type, initializer ? initializer->GetValue() : llvm::Constant::getNullValue(type));
}

void csaw::Builder::Gen(const WhileStatement& statement)
{
    const auto parent = m_Builder->GetInsertBlock()->getParent();
    const auto hdr_block = llvm::BasicBlock::Create(*m_Context, "hdr", parent);
    const auto loop_block = llvm::BasicBlock::Create(*m_Context, "loop", parent);
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end", parent);

    m_Builder->CreateBr(hdr_block);
    m_Builder->SetInsertPoint(hdr_block);
    const auto condition = Gen(statement.Condition);
    m_Builder->CreateCondBr(condition->GetValue(), loop_block, end_block);
    m_Builder->SetInsertPoint(loop_block);
    if (statement.Body) Gen(statement.Body);
    m_Builder->CreateBr(hdr_block);
    m_Builder->SetInsertPoint(end_block);
}
