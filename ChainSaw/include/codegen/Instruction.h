#pragma once

#include <codegen/Def.h>

#include <string>

namespace csaw::codegen
{
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

		std::ostream& Print() const;
		virtual std::ostream& Print(std::ostream& out) const = 0;

		InstructionPtr Prev;
		InstructionPtr Next;
	};

	struct CreateVarInst : Instruction
	{
		CreateVarInst(const std::string& name, TypePtr type, ValueRefPtr value);

		std::ostream& Print(std::ostream& out) const override;

		std::string Name;
		TypePtr Type;
		ValueRefPtr Value;
	};

	struct RetInst : Instruction
	{
		RetInst(ValueRefPtr value);

		bool IsTerminator() const override;

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Value;
	};

	typedef std::shared_ptr<struct Branch> BranchPtr;
	struct SplitInst : Instruction
	{
		SplitInst(ValueRefPtr condition, BranchPtr _true, BranchPtr _false);
		bool IsTerminator() const override;

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Condition;
		BranchPtr True;
		BranchPtr False;
	};

	struct FlowInst : Instruction
	{
		FlowInst(BranchPtr branch);
		bool IsTerminator() const override;

		std::ostream& Print(std::ostream& out) const override;

		BranchPtr Branch;
	};

	typedef std::shared_ptr<struct Function> FunctionPtr;
	struct CallInst : Instruction
	{
		CallInst(FunctionPtr function, ValueRefPtr callee, const std::vector<ValueRefPtr>& args, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		FunctionPtr Function;
		ValueRefPtr Callee;
		std::vector<ValueRefPtr> Args;
		ValueRefPtr Result;
	};

	struct GetElementInst : Instruction
	{
		GetElementInst(ValueRefPtr thing, const std::string& element, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Thing;
		std::string Element;
		ValueRefPtr Result;
	};

	struct AssignVarInst : Instruction
	{
		AssignVarInst(ValueRefPtr var, ValueRefPtr value);

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Var;
		ValueRefPtr Value;
	};

	struct AddInst : Instruction
	{
		AddInst(MathInstMode mode, ValueRefPtr left, ValueRefPtr right, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		MathInstMode Mode;
		ValueRefPtr Left;
		ValueRefPtr Right;
		ValueRefPtr Result;
	};

	struct SubInst : Instruction
	{
		SubInst(MathInstMode mode, ValueRefPtr left, ValueRefPtr right, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		MathInstMode Mode;
		ValueRefPtr Left;
		ValueRefPtr Right;
		ValueRefPtr Result;
	};

	struct MulInst : Instruction
	{
		MulInst(ValueRefPtr left, ValueRefPtr right, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Left;
		ValueRefPtr Right;
		ValueRefPtr Result;
	};

	struct DivInst : Instruction
	{
		DivInst(ValueRefPtr left, ValueRefPtr right, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Left;
		ValueRefPtr Right;
		ValueRefPtr Result;
	};

	struct NegInst : Instruction
	{
		NegInst(ValueRefPtr value, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Value;
		ValueRefPtr Result;
	};

	struct CmpInst : Instruction
	{
		CmpInst(MathInstMode mode, CmpInstMode cmpmode, ValueRefPtr left, ValueRefPtr right, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		MathInstMode Mode;
		CmpInstMode CMPMode;
		ValueRefPtr Left;
		ValueRefPtr Right;
		ValueRefPtr Result;
	};

	struct SelInst : Instruction
	{
		SelInst(ValueRefPtr condition, ValueRefPtr _true, ValueRefPtr _false, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Condition;
		ValueRefPtr True;
		ValueRefPtr False;
		ValueRefPtr Result;
	};

	struct LAndInst : Instruction
	{
		LAndInst(ValueRefPtr left, ValueRefPtr right, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Left;
		ValueRefPtr Right;
		ValueRefPtr Result;
	};

	struct ShLInst : Instruction
	{
		ShLInst(ValueRefPtr left, ValueRefPtr right, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Left;
		ValueRefPtr Right;
		ValueRefPtr Result;
	};

	struct AndInst : Instruction
	{
		AndInst(ValueRefPtr left, ValueRefPtr right, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Left;
		ValueRefPtr Right;
		ValueRefPtr Result;
	};

	struct OrInst : Instruction
	{
		OrInst(ValueRefPtr left, ValueRefPtr right, ValueRefPtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValueRefPtr Left;
		ValueRefPtr Right;
		ValueRefPtr Result;
	};
}
