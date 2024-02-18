#pragma once

#include <codegen/Context.h>
#include <codegen/Value.h>
#include <lang/Def.h>

namespace csaw::codegen
{
	void CodeGen(ContextPtr context, csaw::lang::StmtPtr ptr);
	void CodeGen(ContextPtr context, csaw::lang::FunStmtPtr ptr);
	void CodeGen(ContextPtr context, csaw::lang::RetStmtPtr ptr);
	void CodeGen(ContextPtr context, csaw::lang::EnclosedStmtPtr ptr);
	void CodeGen(ContextPtr context, csaw::lang::ForStmtPtr ptr);
	void CodeGen(ContextPtr context, csaw::lang::VarStmtPtr ptr);
	void CodeGen(ContextPtr context, csaw::lang::IncStmtPtr ptr);
	void CodeGen(ContextPtr context, csaw::lang::WhileStmtPtr ptr);
	void CodeGen(ContextPtr context, csaw::lang::IfStmtPtr ptr);
	void CodeGen(ContextPtr context, csaw::lang::ThingStmtPtr ptr);
	void CodeGen(ContextPtr context, csaw::lang::AliasStmtPtr ptr);
}
