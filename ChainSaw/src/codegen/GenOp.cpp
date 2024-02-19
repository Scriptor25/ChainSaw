#include <codegen/Assert.h>
#include <codegen/GenOp.h>

csaw::codegen::ValuePtr csaw::codegen::CodeGenAssign(ContextPtr context, ValuePtr assignee, ValuePtr value)
{
	return context->CreateAssign(assignee, value);
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpLT(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateCmpLT_NN(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpGT(ContextPtr context, ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpLE(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateCmpLE_NN(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpGE(ContextPtr context, ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpEQ(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateCmpEQ_NN(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpNE(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsStr())
			return context->CreateCmpNE_SS(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpAdd(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateAddNN(left, right);
		if (left->Type->IsChr())
			return context->CreateAddCC(left, right);
		if (left->Type->IsStr())
			return context->CreateAddSS(left, right);
	}

	if (left->Type->IsNum() && right->Type->IsStr())
		return context->CreateAddNS(left, right);
	if (left->Type->IsChr() && right->Type->IsStr())
		return context->CreateAddCS(left, right);
	if (left->Type->IsStr() && right->Type->IsNum())
		return context->CreateAddSN(left, right);
	if (left->Type->IsStr() && right->Type->IsChr())
		return context->CreateAddSC(left, right);

	if (left->Type->IsChr() && right->Type->IsNum())
		return context->CreateAddCN(left, right);
	if (left->Type->IsNum() && right->Type->IsChr())
		return context->CreateAddNC(left, right);

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpSub(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateSubNN(left, right);
		if (left->Type->IsChr())
			return context->CreateSubCC(left, right);
	}

	if (left->Type->IsChr() && right->Type->IsNum())
		return context->CreateSubCN(left, right);
	if (left->Type->IsNum() && right->Type->IsChr())
		return context->CreateSubNC(left, right);

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpMul(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateMul(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpDiv(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateDiv(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpRem(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateRem(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpAnd(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateAnd(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpOr(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateOr(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpXOr(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateXOr(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpLAnd(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateLAnd(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpLOr(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateLOr(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpShL(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateShL(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpShR(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type->IsNum())
			return context->CreateShR(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpNeg(ContextPtr context, ValuePtr value)
{
	if (value->Type->IsNum())
		return context->CreateNeg(value);

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpNot(ContextPtr context, ValuePtr value)
{
	if (value->Type->IsNum())
		return context->CreateNot(value);

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpInv(ContextPtr context, ValuePtr value)
{
	if (value->Type->IsNum())
		return context->CreateInv(value);

	throw;
}
