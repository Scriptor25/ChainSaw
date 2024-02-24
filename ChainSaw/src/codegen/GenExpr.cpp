#include <codegen/Assert.h>
#include <codegen/GenExpr.h>
#include <codegen/GenOp.h>

#include <lang/Expr.h>

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::ExprPtr ptr)
{
	if (auto p = std::dynamic_pointer_cast<csaw::lang::CallExpr>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::NumExpr>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::ChrExpr>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::StrExpr>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::IdentExpr>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::BinExpr>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::UnExpr>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::IndexExpr>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::MemberExpr>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::VarArgExpr>(ptr))
		return CodeGen(context, p);
	if (auto p = std::dynamic_pointer_cast<csaw::lang::SelExpr>(ptr))
		return CodeGen(context, p);

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::CallExprPtr ptr)
{
	std::string name;
	ValuePtr callee;

	if (auto p = std::dynamic_pointer_cast<csaw::lang::IdentExpr>(ptr->Callee))
		name = p->Id;
	else if (auto p = std::dynamic_pointer_cast<csaw::lang::MemberExpr>(ptr->Callee))
	{
		name = p->Member;
		callee = CodeGen(context, p->Object);
	}

	std::vector<ValuePtr> args;
	std::vector<TypePtr> argtypes;
	for (auto& arg : ptr->Args)
	{
		auto value = CodeGen(context, arg);
		args.push_back(value);
		argtypes.push_back(value->Type);
	}

	auto function = context->GetFunction(name, callee ? callee->Type : context->GetEmptyType(), argtypes);
	Assert(function, context, ptr);
	return context->CreateCall(function, callee, args);
}

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::NumExprPtr ptr)
{
	return context->GetConstNum(ptr->Value);
}

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::ChrExprPtr ptr)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::StrExprPtr ptr)
{
	return context->GetConstStr(ptr->Value);
}

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::IdentExprPtr ptr)
{
	return context->GetVariable(ptr->Id);
}

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::BinExprPtr ptr)
{
	auto left = CodeGen(context, ptr->Left);
	auto right = CodeGen(context, ptr->Right);
	std::string op = ptr->Operator;

	if (auto function = context->GetFunction(op, context->GetEmptyType(), { left->Type, right->Type }))
		return context->CreateCall(function, nullptr, { left, right });

	if (op == "=") return CodeGenAssign(context, left, right);

	if (op == "<") return CodeGenOpLT(context, left, right);
	if (op == ">") return CodeGenOpGT(context, left, right);
	if (op == "<=") return CodeGenOpLE(context, left, right);
	if (op == ">=") return CodeGenOpGE(context, left, right);
	if (op == "==") return CodeGenOpEQ(context, left, right);
	if (op == "!=") return CodeGenOpNE(context, left, right);

	if (auto function = context->GetFunction(op, left->Type, { right->Type }))
		return context->CreateCall(function, left, { right });

	size_t assignpos = op.find('=');
	bool assign = assignpos != std::string::npos;
	auto o = op.substr(0, assignpos);

	ValuePtr value;
	if (o == "+") value = CodeGenOpAdd(context, left, right);
	else if (o == "-") value = CodeGenOpSub(context, left, right);
	else if (o == "*") value = CodeGenOpMul(context, left, right);
	else if (o == "/") value = CodeGenOpDiv(context, left, right);
	else if (o == "%") value = CodeGenOpRem(context, left, right);
	else if (o == "&") value = CodeGenOpAnd(context, left, right);
	else if (o == "|") value = CodeGenOpOr(context, left, right);
	else if (o == "^") value = CodeGenOpXOr(context, left, right);
	else if (o == "&&") value = CodeGenOpLAnd(context, left, right);
	else if (o == "||") value = CodeGenOpLOr(context, left, right);
	else if (o == "<<") value = CodeGenOpShL(context, left, right);
	else if (o == ">>") value = CodeGenOpShR(context, left, right);

	Assert(value, context, ptr);
	if (assign) return CodeGenAssign(context, left, value);
	return value;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::UnExprPtr ptr)
{
	auto value = CodeGen(context, ptr->Value);
	std::string op = ptr->Operator;

	std::vector<ValuePtr> arg;
	std::vector<TypePtr> argtype;

	if (ptr->RightOp)
	{
		arg.push_back(value);
		argtype.push_back(value->Type);
	}

	if (auto function = context->GetFunction(op, ptr->RightOp ? context->GetEmptyType() : value->Type, argtype))
		return context->CreateCall(function, ptr->RightOp ? nullptr : value, arg);

	if (op == "++")
	{
		auto val = CodeGenOpAdd(context, value, context->GetConstNum(1));
		if (ptr->RightOp)
		{
			CodeGenAssign(context, value, val);
			return val;
		}
		return CodeGenAssign(context, value, val);
	}
	if (op == "--")
	{
		auto val = CodeGenOpSub(context, value, context->GetConstNum(1));
		if (ptr->RightOp)
		{
			CodeGenAssign(context, value, val);
			return val;
		}
		return CodeGenAssign(context, value, val);
	}
	if (op == "-") return CodeGenOpNeg(context, value);
	if (op == "!") return CodeGenOpNot(context, value);
	if (op == "~") return CodeGenOpInv(context, value);

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::IndexExprPtr ptr)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::MemberExprPtr ptr)
{
	auto thing = CodeGen(context, ptr->Object);
	return context->CreateGetElement(thing, ptr->Member);
}

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::VarArgExprPtr ptr)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGen(ContextPtr context, csaw::lang::SelExprPtr ptr)
{
	auto tbr = context->CreateBranch();
	auto fbr = context->CreateBranch();
	auto ebr = context->CreateBranch();

	auto condition = CodeGen(context, ptr->Condition);
	context->CreateSplit(condition, tbr, fbr);

	context->SetInsertPoint(tbr);
	auto tval = CodeGen(context, ptr->True);
	context->CreateFlow(ebr);

	context->SetInsertPoint(fbr);
	auto fval = CodeGen(context, ptr->False);
	context->CreateFlow(ebr);

	context->SetInsertPoint(ebr);

	Assert(tval->Type == fval->Type, context, ptr);
	auto merge = context->CreateSel(condition, tval->Type, tval, fval);

	return merge;
}
