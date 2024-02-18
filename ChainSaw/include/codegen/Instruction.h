#pragma once

#include <codegen/Type.h>
#include <codegen/Value.h>

#include <memory>
#include <string>

namespace csaw::codegen
{
	struct Instruction;

	struct CreateVarInst;
	struct RetInst;
	struct SplitInst;
	struct FlowInst;

	struct CallInst;
	struct GetVarInst;
	struct GetElementInst;
	struct AssignVarInst;
	struct AddInst;
	struct SubInst;
	struct MulInst;
	struct DivInst;
	struct NegInst;
	struct CmpInst;
	struct MergeInst;

	typedef std::shared_ptr<Instruction> InstructionPtr;

	typedef std::shared_ptr<CreateVarInst> CreateVarInstPtr;
	typedef std::shared_ptr<RetInst> RetInstPtr;
	typedef std::shared_ptr<SplitInst> SplitInstPtr;
	typedef std::shared_ptr<FlowInst> FlowInstPtr;

	typedef std::shared_ptr<CallInst> CallInstPtr;
	typedef std::shared_ptr<GetElementInst> GetElementInstPtr;
	typedef std::shared_ptr<AssignVarInst> AssignVarInstPtr;
	typedef std::shared_ptr<AddInst> AddInstPtr;
	typedef std::shared_ptr<SubInst> SubInstPtr;
	typedef std::shared_ptr<MulInst> MulInstPtr;
	typedef std::shared_ptr<DivInst> DivInstPtr;
	typedef std::shared_ptr<NegInst> NegInstPtr;
	typedef std::shared_ptr<CmpInst> CmpInstPtr;
	typedef std::shared_ptr<MergeInst> MergeInstPtr;

	enum MathInstMode
	{
		MODE_NN,
		MODE_CC,
		MODE_SS,
		MODE_NS,
		MODE_CS,
		MODE_SN,
		MODE_SC,
		MODE_CN,
		MODE_NC,
	};

	enum CmpInstMode
	{
		CMP_LT,
		CMP_GT,
		CMP_LE,
		CMP_GE,
		CMP_EQ,
		CMP_NE,
	};

	struct Instruction
		: std::enable_shared_from_this<Instruction>
	{
		virtual ~Instruction() {}
		virtual void Insert(InstructionPtr instruction);
		virtual bool IsTerminator() const;

		InstructionPtr Prev;
		InstructionPtr Next;
	};

	struct CreateVarInst : Instruction
	{
		CreateVarInst(const std::string& name, TypePtr type, ValuePtr value);

		std::string Name;
		TypePtr Type;
		ValuePtr Value;
	};

	struct RetInst : Instruction
	{
		RetInst(ValuePtr value);
		bool IsTerminator() const override;

		ValuePtr Value;
	};

	typedef std::shared_ptr<struct Branch> BranchPtr;
	struct SplitInst : Instruction
	{
		SplitInst(ValuePtr condition, BranchPtr _true, BranchPtr _false);
		bool IsTerminator() const override;

		ValuePtr Condition;
		BranchPtr True;
		BranchPtr False;
	};

	struct FlowInst : Instruction
	{
		FlowInst(BranchPtr branch);
		bool IsTerminator() const override;

		BranchPtr Branch;
	};

	typedef std::shared_ptr<struct Function> FunctionPtr;
	struct CallInst : Instruction
	{
		CallInst(FunctionPtr function, ValuePtr callee, const std::vector<ValuePtr>& args, ValuePtr result);

		FunctionPtr Function;
		ValuePtr Callee;
		std::vector<ValuePtr> Args;
		ValuePtr Result;
	};

	struct GetVarInst : Instruction
	{
		GetVarInst(const std::string& name, ValuePtr result);

		std::string Name;
		ValuePtr Result;
	};

	struct GetElementInst : Instruction
	{
		GetElementInst(ValuePtr thing, const std::string& element, ValuePtr result);

		ValuePtr Thing;
		std::string Element;
		ValuePtr Result;
	};

	struct AssignVarInst : Instruction
	{
		AssignVarInst(ValuePtr var, ValuePtr value);

		ValuePtr Var;
		ValuePtr Value;
	};

	struct AddInst : Instruction
	{
		AddInst(MathInstMode mode, ValuePtr left, ValuePtr right, ValuePtr result);

		MathInstMode Mode;
		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct SubInst : Instruction
	{
		SubInst(MathInstMode mode, ValuePtr left, ValuePtr right, ValuePtr result);

		MathInstMode Mode;
		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct MulInst : Instruction
	{
		MulInst(ValuePtr left, ValuePtr right, ValuePtr result);

		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct DivInst : Instruction
	{
		DivInst(ValuePtr left, ValuePtr right, ValuePtr result);

		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct NegInst : Instruction
	{
		NegInst(ValuePtr value, ValuePtr result);

		ValuePtr Value;
		ValuePtr Result;
	};

	struct CmpInst : Instruction
	{
		CmpInst(CmpInstMode mode, ValuePtr left, ValuePtr right, ValuePtr result);

		CmpInstMode Mode;
		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct MergeInst : Instruction
	{
		MergeInst(ValuePtr condition, BranchPtr _true, BranchPtr _false, ValuePtr result);

		ValuePtr Condition;
		BranchPtr True;
		BranchPtr False;
		ValuePtr Result;
	};
}
