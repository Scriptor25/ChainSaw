#include <codegen/CodeGen.h>
#include <codegen/Context.h>
#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

#include <lang/Expr.h>
#include <lang/Parser.h>
#include <lang/Stmt.h>

#include <filesystem>
#include <fstream>

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::StmtPtr ptr)
{
	if (auto p = std::dynamic_pointer_cast<csaw::lang::FunStmt>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::RetStmt>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::EnclosedStmt>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::ForStmt>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::VarStmt>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::IncStmt>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::WhileStmt>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::IfStmt>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::ThingStmt>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::AliasStmt>(ptr))
		return CodeGen(context, p);

	if (auto p = std::dynamic_pointer_cast<csaw::lang::Expr>(ptr))
	{
		CodeGen(context, p);
		return;
	}

	throw;
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::FunStmtPtr ptr)
{
	auto callee = context->GetType(ptr->Callee);
	auto result = context->GetType(ptr->Result);
	Assert(result, context, ptr);

	std::vector<TypePtr> argtypes;
	for (auto& arg : ptr->Args)
		argtypes.push_back(context->GetType(arg.second));

	auto type = context->GetFunctionType(result, argtypes, ptr->VarArg);
	auto function = context->GetFunction(ptr->Name, callee, type, ptr->Constructor);

	for (size_t i = 0; i < function->Args.size(); i++)
		function->Args[i]->Name = ptr->Args[i].first;

	if (!ptr->Body)
		return;

	Assert(!function->Next, context, ptr);

	context->ClearVariables();
	context->CreateArgs(function->Args);

	context->SetInsertPoint(function);

	if (function->IsConstructor)
		context->CreateVar("my", function->Type->Result);
	if (!function->Callee->IsEmpty())
		context->CreateVar("my", function->Callee);

	CodeGen(context, ptr->Body);
	if (function->IsConstructor)
		context->CreateRet(context->GetVariable("my"));

	context->SetInsertGlobal();
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::RetStmtPtr ptr)
{
	if (!ptr->Value)
	{
		context->CreateEmptyRet();
		return;
	}

	auto value = CodeGen(context, ptr->Value);
	context->CreateRet(value);
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::EnclosedStmtPtr ptr)
{
	for (auto& stmt : ptr->Content)
		CodeGen(context, stmt);
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::ForStmtPtr ptr)
{
	auto cbr = context->CreateBranch();
	auto lbr = context->CreateBranch();
	auto ebr = context->CreateBranch();

	if (ptr->Pre) CodeGen(context, ptr->Pre);
	context->CreateFlow(cbr);

	context->SetInsertPoint(cbr);
	auto condition = CodeGen(context, ptr->Condition);
	context->CreateSplit(condition, lbr, ebr);

	context->SetInsertPoint(lbr);
	CodeGen(context, ptr->Body);
	if (ptr->Loop) CodeGen(context, ptr->Loop);
	context->CreateFlow(cbr);

	context->SetInsertPoint(ebr);
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::VarStmtPtr ptr)
{
	auto type = context->GetType(ptr->Type);
	auto value =
		ptr->Value
		? CodeGen(context, ptr->Value)
		: std::make_shared<Value>(type);
	context->CreateVar(ptr->Name, type, value);
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::IncStmtPtr ptr)
{
	auto path = std::filesystem::absolute(context->Filepath().parent_path() / ptr->Filename);

	std::ifstream stream(path);
	Assert(stream.is_open(), context, ptr);

	context->PushFilepath(path);
	csaw::lang::Parser::Parse(
		stream,
		[context](csaw::lang::StmtPtr ptr)
		{
#ifdef PRINT_PARSE
			std::cout << ptr << std::endl;
#endif
			CodeGen(context, ptr);
		});
	context->PopFilepath();
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::WhileStmtPtr ptr)
{
	auto cbr = context->CreateBranch();
	auto lbr = context->CreateBranch();
	auto ebr = context->CreateBranch();

	context->CreateFlow(cbr);

	context->SetInsertPoint(cbr);
	auto condition = CodeGen(context, ptr->Condition);
	context->CreateSplit(condition, lbr, ebr);

	context->SetInsertPoint(lbr);
	CodeGen(context, ptr->Body);
	context->CreateFlow(cbr);

	context->SetInsertPoint(ebr);
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::IfStmtPtr ptr)
{
	auto tbr = context->CreateBranch();
	auto fbr = ptr->False ? context->CreateBranch() : nullptr;
	auto ebr = context->CreateBranch();

	auto condition = CodeGen(context, ptr->Condition);
	context->CreateSplit(condition, tbr, ptr->False ? fbr : ebr);

	context->SetInsertPoint(tbr);
	CodeGen(context, ptr->True);
	if (!context->GetInsertBranch()->HasTerminator())
		context->CreateFlow(ebr);

	if (ptr->False)
	{
		context->SetInsertPoint(fbr);
		CodeGen(context, ptr->False);
		if (!context->GetInsertBranch()->HasTerminator())
			context->CreateFlow(ebr);
	}

	context->SetInsertPoint(ebr);
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::ThingStmtPtr ptr)
{
	auto type = context->GetThingType(ptr->Name);
	Assert(type->Elements.empty(), context, ptr);

	for (auto& element : ptr->Elements)
		type->Elements[element.first] = context->GetType(element.second);
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::AliasStmtPtr ptr)
{
	throw;
}
