#pragma once

#include <codegen/Context.h>

#include <lang/Def.h>

namespace csaw::codegen
{
	void WAssert(const char* expr, ContextPtr context, csaw::lang::StmtPtr ptr);
#define Assert(expression, context, ptr) (void)((!!(expression)) || (WAssert(#expression, context, ptr), 0))
}
