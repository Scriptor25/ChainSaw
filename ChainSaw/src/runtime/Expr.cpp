#include <runtime/Compute.h>
#include <runtime/Const.h>
#include <runtime/Operators.h>
#include <lang/Expr.h>

#include <cassert>

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::ExprPtr expr)
{
	if (auto e = std::dynamic_pointer_cast<csaw::lang::CallExpr>(expr))
		return Evaluate(env, e);
	if (auto e = std::dynamic_pointer_cast<csaw::lang::NumExpr>(expr))
		return Evaluate(env, e);
	if (auto e = std::dynamic_pointer_cast<csaw::lang::ChrExpr>(expr))
		return Evaluate(env, e);
	if (auto e = std::dynamic_pointer_cast<csaw::lang::StrExpr>(expr))
		return Evaluate(env, e);
	if (auto e = std::dynamic_pointer_cast<csaw::lang::IdentExpr>(expr))
		return Evaluate(env, e);
	if (auto e = std::dynamic_pointer_cast<csaw::lang::BinExpr>(expr))
		return Evaluate(env, e);
	if (auto e = std::dynamic_pointer_cast<csaw::lang::UnExpr>(expr))
		return Evaluate(env, e);
	if (auto e = std::dynamic_pointer_cast<csaw::lang::IndexExpr>(expr))
		return Evaluate(env, e);
	if (auto e = std::dynamic_pointer_cast<csaw::lang::MemberExpr>(expr))
		return Evaluate(env, e);
	if (auto e = std::dynamic_pointer_cast<csaw::lang::VarArgExpr>(expr))
		return Evaluate(env, e);
	if (auto e = std::dynamic_pointer_cast<csaw::lang::SelExpr>(expr))
		return Evaluate(env, e);

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::CallExprPtr expr)
{
	std::string name;
	ValuePtr callee;

	if (auto e = std::dynamic_pointer_cast<csaw::lang::IdentExpr>(expr->Callee))
	{
		name = e->Id;
	}
	else if (auto e = std::dynamic_pointer_cast<csaw::lang::MemberExpr>(expr->Callee))
	{
		name = e->Member;
		callee = Evaluate(env, e->Object);
	}

	std::vector<ValuePtr> args;
	for (auto& arg : expr->Args)
		args.push_back(Evaluate(env, arg));

	return Environment::GetAndInvoke(env->GetGlobal(), name, callee, args);
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::NumExprPtr expr)
{
	return ConstNum::Ptr(expr->Value);
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::ChrExprPtr expr)
{
	return ConstChr::Ptr(expr->Value);
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::StrExprPtr expr)
{
	return ConstStr::Ptr(expr->Value);
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::IdentExprPtr expr)
{
	return env->GetVariable(expr->Id);
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::BinExprPtr expr)
{
	auto l = Evaluate(env, expr->Left);
	auto r = Evaluate(env, expr->Right);
	std::string oop = expr->Operator;

	if (auto function = Environment::GetFunction(oop, "", { l->GetType(), r->GetType() }))
		return function->Definition(env->GetGlobal(), ValuePtr(), { l, r });

	if (oop == "=") return Assign(env, expr->Left, r);

	if (oop == "<") return EvalOpLT(env, l, r);
	if (oop == ">") return EvalOpGT(env, l, r);
	if (oop == "<=") return EvalOpLE(env, l, r);
	if (oop == ">=") return EvalOpGE(env, l, r);
	if (oop == "==") return EvalOpEQ(env, l, r);
	if (oop == "!=") return EvalOpNE(env, l, r);

	if (auto function = Environment::GetFunction(oop, l->GetType(), { r->GetType() }))
		return function->Definition(env->GetGlobal(), l, { r });

	size_t assignpos = oop.find('=');
	bool assign = assignpos != std::string::npos;
	auto op = oop.substr(0, assignpos);

	ValuePtr value;
	if (op == "+") value = EvalOpAdd(env, l, r);
	else if (op == "-") value = EvalOpSub(env, l, r);
	else if (op == "*") value = EvalOpMul(env, l, r);
	else if (op == "/") value = EvalOpDiv(env, l, r);
	else if (op == "%") value = EvalOpRem(env, l, r);

	else if (op == "&") value = EvalOpAnd(env, l, r);
	else if (op == "|") value = EvalOpOr(env, l, r);
	else if (op == "^") value = EvalOpXOr(env, l, r);

	else if (op == "&&") value = EvalOpLAnd(env, l, r);
	else if (op == "||") value = EvalOpLOr(env, l, r);

	else if (op == "<<") value = EvalOpShL(env, l, r);
	else if (op == ">>") value = EvalOpShR(env, l, r);

	assert(value);
	if (assign)
		return Assign(env, expr->Left, value);
	return value;
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::UnExprPtr expr)
{
	auto value = Evaluate(env, expr->Value);
	std::string op = expr->Operator;
	bool r = expr->RightOp;

	std::vector<std::string> type;
	std::vector<ValuePtr> arg;
	if (!r)
	{
		type.push_back(value->GetType());
		arg.push_back(value);
	}

	if (auto function = Environment::GetFunction(op, r ? value->GetType() : "", type))
		return function->Definition(env->GetGlobal(), r ? value : ValuePtr(), arg);

	if (op == "++")
	{
		auto v = EvalOpAddOne(env, value);
		if (r)
		{
			Assign(env, expr->Value, v);
			return v;
		}
		return Assign(env, expr->Value, v);
	}
	if (op == "--")
	{
		auto v = EvalOpSubOne(env, value);
		if (r)
		{
			Assign(env, expr->Value, v);
			return v;
		}
		return Assign(env, expr->Value, v);
	}
	if (op == "-") return EvalOpNeg(env, value);
	if (op == "!") return EvalOpLNot(env, value);
	if (op == "~") return EvalOpNot(env, value);

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::IndexExprPtr expr)
{
	throw;
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::MemberExprPtr expr)
{
	auto object = Evaluate(env, expr->Object);
	return object->GetElement(expr->Member);
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::VarArgExprPtr expr)
{
	std::string type = expr->Type;
	auto index = size_t(Evaluate(env, expr->Index)->GetNum());

	auto value = env->GetVarArg(index, type);
	assert(value);
	return value;
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::SelExprPtr expr)
{
	return EvalOpBool(env, Evaluate(env, expr->Condition))
		? Evaluate(env, expr->True)
		: Evaluate(env, expr->False);
}
