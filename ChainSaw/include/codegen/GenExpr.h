#pragma once

#include <codegen/Def.h>
#include <lang/Def.h>

namespace csaw::codegen
{
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::ExprPtr ptr);
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::CallExprPtr ptr);
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::NumExprPtr ptr);
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::ChrExprPtr ptr);
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::StrExprPtr ptr);
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::IdentExprPtr ptr);
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::BinExprPtr ptr);
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::UnExprPtr ptr);
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::IndexExprPtr ptr);
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::MemberExprPtr ptr);
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::VarArgExprPtr ptr);
	ValueRefPtr CodeGen(ContextPtr context, csaw::lang::SelExprPtr ptr);
}
