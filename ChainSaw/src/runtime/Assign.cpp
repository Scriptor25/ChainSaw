#include <runtime/Compute.h>
#include <lang/Expr.h>

csaw::runtime::ValuePtr csaw::runtime::Assign(EnvironmentPtr env, const csaw::lang::ExprPtr expr, const ValuePtr value)
{
	if (auto e = std::dynamic_pointer_cast<csaw::lang::IdentExpr>(expr))
		return env->SetVariable(e->Id, value);

	if (auto e = std::dynamic_pointer_cast<csaw::lang::MemberExpr>(expr))
	{
		auto object = Evaluate(env, e->Object);
		return object->SetElement(e->Member, value);
	}

	throw;
}
