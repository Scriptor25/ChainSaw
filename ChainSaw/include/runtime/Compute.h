#pragma once

#include <runtime/Runtime.h>
#include <lang/ASTDef.h>

namespace csaw::runtime
{
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::StmtPtr stmt);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::FunStmtPtr stmt);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::RetStmtPtr stmt);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::EnclosedStmtPtr stmt);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::ForStmtPtr stmt);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::VarStmtPtr stmt);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::IncStmtPtr stmt);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::WhileStmtPtr stmt);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::IfStmtPtr stmt);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::ThingStmtPtr stmt);

	ValuePtr Assign(EnvironmentPtr env, const csaw::lang::ExprPtr expr, const ValuePtr value);

	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::ExprPtr expr);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::CallExprPtr expr);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::NumExprPtr expr);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::ChrExprPtr expr);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::StrExprPtr expr);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::IdentExprPtr expr);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::BinExprPtr expr);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::UnExprPtr expr);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::IndexExprPtr expr);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::MemberExprPtr expr);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::VarArgExprPtr expr);
	ValuePtr Evaluate(EnvironmentPtr env, const csaw::lang::SelExprPtr expr);
}
