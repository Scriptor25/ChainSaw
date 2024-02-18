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
		if (left->Type == context->GetNumType())
			return context->CreateCmpLT(left, right);
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
		if (left->Type == context->GetNumType())
			return context->CreateCmpLE(left, right);
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
		if (left->Type == context->GetNumType())
			return context->CreateCmpEQ(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpNE(ContextPtr context, ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpAdd(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateAddNN(left, right);
		if (left->Type == context->GetChrType())
			return context->CreateAddCC(left, right);
		if (left->Type == context->GetStrType())
			return context->CreateAddSS(left, right);
	}

	if (left->Type == context->GetNumType() && right->Type == context->GetStrType())
		return context->CreateAddNS(left, right);
	if (left->Type == context->GetChrType() && right->Type == context->GetStrType())
		return context->CreateAddCS(left, right);
	if (left->Type == context->GetStrType() && right->Type == context->GetNumType())
		return context->CreateAddSN(left, right);
	if (left->Type == context->GetStrType() && right->Type == context->GetChrType())
		return context->CreateAddSC(left, right);

	if (left->Type == context->GetChrType() && right->Type == context->GetNumType())
		return context->CreateAddCN(left, right);
	if (left->Type == context->GetNumType() && right->Type == context->GetChrType())
		return context->CreateAddNC(left, right);

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpSub(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateSubNN(left, right);
		if (left->Type == context->GetChrType())
			return context->CreateSubCC(left, right);
	}

	if (left->Type == context->GetChrType() && right->Type == context->GetNumType())
		return context->CreateSubCN(left, right);
	if (left->Type == context->GetNumType() && right->Type == context->GetChrType())
		return context->CreateSubNC(left, right);

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpMul(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateMul(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpDiv(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateDiv(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpRem(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateRem(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpAnd(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateAnd(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpOr(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateOr(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpXOr(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateXOr(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpLAnd(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateLAnd(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpLOr(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateLOr(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpShL(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateShL(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpShR(ContextPtr context, ValuePtr left, ValuePtr right)
{
	if (left->Type == right->Type)
	{
		if (left->Type == context->GetNumType())
			return context->CreateShR(left, right);
	}

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpNeg(ContextPtr context, ValuePtr value)
{
	if (value->Type == context->GetNumType())
		return context->CreateNeg(value);

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpNot(ContextPtr context, ValuePtr value)
{
	if (value->Type == context->GetNumType())
		return context->CreateNot(value);

	throw;
}

csaw::codegen::ValuePtr csaw::codegen::CodeGenOpInv(ContextPtr context, ValuePtr value)
{
	if (value->Type == context->GetNumType())
		return context->CreateInv(value);

	throw;
}
