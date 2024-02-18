#pragma once

#include <codegen/Context.h>
#include <codegen/Value.h>
#include <lang/Def.h>

namespace csaw::codegen
{
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
}
