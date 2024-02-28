#pragma once

#include <codegen/Def.h>
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

	ValuePtr CodeGen(ContextPtr context, csaw::lang::ExprPtr ptr);
	ValuePtr CodeGen(ContextPtr context, csaw::lang::CallExprPtr ptr);
	ValuePtr CodeGen(ContextPtr context, csaw::lang::NumExprPtr ptr);
	ValuePtr CodeGen(ContextPtr context, csaw::lang::ChrExprPtr ptr);
	ValuePtr CodeGen(ContextPtr context, csaw::lang::StrExprPtr ptr);
	ValuePtr CodeGen(ContextPtr context, csaw::lang::IdentExprPtr ptr);
	ValuePtr CodeGen(ContextPtr context, csaw::lang::BinExprPtr ptr);
	ValuePtr CodeGen(ContextPtr context, csaw::lang::UnExprPtr ptr);
	ValuePtr CodeGen(ContextPtr context, csaw::lang::IndexExprPtr ptr);
	ValuePtr CodeGen(ContextPtr context, csaw::lang::MemberExprPtr ptr);
	ValuePtr CodeGen(ContextPtr context, csaw::lang::VarArgExprPtr ptr);
	ValuePtr CodeGen(ContextPtr context, csaw::lang::SelExprPtr ptr);

	ValuePtr CodeGenAssign(ContextPtr context, ValuePtr assignee, ValuePtr value);

	ValuePtr CodeGenOpLT(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpGT(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpLE(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpGE(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpEQ(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpNE(ContextPtr context, ValuePtr left, ValuePtr right);

	ValuePtr CodeGenOpAdd(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpSub(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpMul(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpDiv(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpRem(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpAnd(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpOr(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpXOr(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpLAnd(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpLOr(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpShL(ContextPtr context, ValuePtr left, ValuePtr right);
	ValuePtr CodeGenOpShR(ContextPtr context, ValuePtr left, ValuePtr right);

	ValuePtr CodeGenOpNeg(ContextPtr context, ValuePtr value);
	ValuePtr CodeGenOpNot(ContextPtr context, ValuePtr value);
	ValuePtr CodeGenOpInv(ContextPtr context, ValuePtr value);
}
