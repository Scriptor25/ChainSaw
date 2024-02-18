#include <codegen/Assert.h>
#include <codegen/GenExpr.h>
#include <codegen/GenStmt.h>

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
	for (auto& arg : ptr->Args)
		context->SetVariable(arg.first, context->GetType(arg.second));

	if (function->IsConstructor)
		context->SetVariable("my", function->Type->Result);
	if (function->Callee != context->GetEmptyType())
		context->SetVariable("my", function->Callee);

	context->SetInsertPoint(function);
	CodeGen(context, ptr->Body);
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
	throw;
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::VarStmtPtr ptr)
{
	auto type = context->GetType(ptr->Type);
	auto value = CodeGen(context, ptr->Value);
	context->CreateVar(ptr->Name, type, value);
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::IncStmtPtr ptr)
{
	auto path = std::filesystem::path(context->Filepath()).parent_path() / ptr->Filename;

	std::ifstream stream(path);
	Assert(stream.is_open(), context, ptr);

	context->PushFilepath(path.string());
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
	throw;
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::IfStmtPtr ptr)
{
	throw;
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::ThingStmtPtr ptr)
{
	std::map<std::string, TypePtr> elements;
	for (auto& element : ptr->Elements)
		elements[element.first] = context->GetType(element.second);
	context->CreateThingType(ptr->Name, elements);
}

void csaw::codegen::CodeGen(ContextPtr context, csaw::lang::AliasStmtPtr ptr)
{
	throw;
}
