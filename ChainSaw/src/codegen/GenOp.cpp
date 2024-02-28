#include <codegen/Assert.h>
#include <codegen/Context.h>
#include <codegen/GenOp.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenAssign(ContextPtr context, ValueRefPtr assignee, ValueRefPtr value)
{
	return context->CreateAssign(assignee, value);
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpLT(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateCmpLT_NN(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpGT(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateCmpGT_NN(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpLE(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateCmpLE_NN(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpGE(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpEQ(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateCmpEQ_NN(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpNE(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsStr())
			return context->CreateCmpNE_SS(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpAdd(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateAddNN(left, right);
		if (left->Type()->IsChr())
			return context->CreateAddCC(left, right);
		if (left->Type()->IsStr())
			return context->CreateAddSS(left, right);
	}

	if (left->Type()->IsNum() && right->Type()->IsStr())
		return context->CreateAddNS(left, right);
	if (left->Type()->IsChr() && right->Type()->IsStr())
		return context->CreateAddCS(left, right);
	if (left->Type()->IsStr() && right->Type()->IsNum())
		return context->CreateAddSN(left, right);
	if (left->Type()->IsStr() && right->Type()->IsChr())
		return context->CreateAddSC(left, right);

	if (left->Type()->IsChr() && right->Type()->IsNum())
		return context->CreateAddCN(left, right);
	if (left->Type()->IsNum() && right->Type()->IsChr())
		return context->CreateAddNC(left, right);

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpSub(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateSubNN(left, right);
		if (left->Type()->IsChr())
			return context->CreateSubCC(left, right);
	}

	if (left->Type()->IsChr() && right->Type()->IsNum())
		return context->CreateSubCN(left, right);
	if (left->Type()->IsNum() && right->Type()->IsChr())
		return context->CreateSubNC(left, right);

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpMul(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateMul(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpDiv(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateDiv(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpRem(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateRem(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpAnd(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateAnd(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpOr(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateOr(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpXOr(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateXOr(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpLAnd(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateLAnd(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpLOr(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateLOr(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpShL(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateShL(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpShR(ContextPtr context, ValueRefPtr left, ValueRefPtr right)
{
	if (left->Type() == right->Type())
	{
		if (left->Type()->IsNum())
			return context->CreateShR(left, right);
	}

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpNeg(ContextPtr context, ValueRefPtr value)
{
	if (value->Type()->IsNum())
		return context->CreateNeg(value);

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpNot(ContextPtr context, ValueRefPtr value)
{
	if (value->Type()->IsNum())
		return context->CreateNot(value);

	throw;
}

csaw::codegen::ValueRefPtr csaw::codegen::CodeGenOpInv(ContextPtr context, ValueRefPtr value)
{
	if (value->Type()->IsNum())
		return context->CreateInv(value);

	throw;
}
