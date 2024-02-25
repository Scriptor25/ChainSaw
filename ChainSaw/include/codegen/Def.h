#pragma once

#include <functional>
#include <memory>

namespace csaw::codegen
{
	struct Type;
	typedef std::shared_ptr<Type> TypePtr;

	struct NumType;
	typedef std::shared_ptr<NumType> NumTypePtr;

	struct ChrType;
	typedef std::shared_ptr<ChrType> ChrTypePtr;

	struct StrType;
	typedef std::shared_ptr<StrType> StrTypePtr;

	struct EmptyType;
	typedef std::shared_ptr<EmptyType> EmptyTypePtr;

	struct ThingType;
	typedef std::shared_ptr<ThingType> ThingTypePtr;

	struct FunctionType;
	typedef std::shared_ptr<FunctionType> FunctionTypePtr;

	struct Value;
	typedef std::shared_ptr<Value> ValuePtr;

	struct Const;
	typedef std::shared_ptr<Const> ConstPtr;

	struct ConstNum;
	typedef std::shared_ptr<ConstNum> ConstNumPtr;

	struct ConstChr;
	typedef std::shared_ptr<ConstChr> ConstChrPtr;

	struct ConstStr;
	typedef std::shared_ptr<ConstStr> ConstStrPtr;

	struct ConstThing;
	typedef std::shared_ptr<ConstThing> ConstThingPtr;

	struct ValueRef;
	typedef std::shared_ptr<ValueRef> ValueRefPtr;

	class Context;
	typedef std::shared_ptr<Context> ContextPtr;

	struct Arg;
	typedef std::shared_ptr<Arg> ArgPtr;

	struct Function;
	typedef std::shared_ptr<Function> FunctionPtr;

	struct Branch;
	typedef std::shared_ptr<Branch> BranchPtr;

	struct Instruction;
	typedef std::shared_ptr<Instruction> InstructionPtr;

	struct CreateVarInst;
	typedef std::shared_ptr<CreateVarInst> CreateVarInstPtr;

	struct RetInst;
	typedef std::shared_ptr<RetInst> RetInstPtr;

	struct SplitInst;
	typedef std::shared_ptr<SplitInst> SplitInstPtr;

	struct FlowInst;
	typedef std::shared_ptr<FlowInst> FlowInstPtr;

	struct CallInst;
	typedef std::shared_ptr<CallInst> CallInstPtr;

	struct GetElementInst;
	typedef std::shared_ptr<GetElementInst> GetElementInstPtr;

	struct AssignVarInst;
	typedef std::shared_ptr<AssignVarInst> AssignVarInstPtr;

	struct AddInst;
	typedef std::shared_ptr<AddInst> AddInstPtr;

	struct SubInst;
	typedef std::shared_ptr<SubInst> SubInstPtr;

	struct MulInst;
	typedef std::shared_ptr<MulInst> MulInstPtr;

	struct DivInst;
	typedef std::shared_ptr<DivInst> DivInstPtr;

	struct NegInst;
	typedef std::shared_ptr<NegInst> NegInstPtr;

	struct CmpInst;
	typedef std::shared_ptr<CmpInst> CmpInstPtr;

	struct SelInst;
	typedef std::shared_ptr<SelInst> SelInstPtr;

	struct LAndInst;
	typedef std::shared_ptr<LAndInst> LAndInstPtr;

	struct ShLInst;
	typedef std::shared_ptr<ShLInst> ShLInstPtr;

	struct AndInst;
	typedef std::shared_ptr<AndInst> AndInstPtr;

	struct OrInst;
	typedef std::shared_ptr<OrInst> OrInstPtr;

	struct NativeInst;
	typedef std::shared_ptr<NativeInst> NativeInstPtr;
	typedef std::function<ValueRefPtr(ContextPtr context, ValueRefPtr callee, const std::vector<ValueRefPtr>& args)> NativeFunction;
}
