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

	struct Function : Instruction
	{
		Function(const std::string& name, FunctionTypePtr type, bool isconstructor, const std::vector<ArgPtr>& args, TypePtr callee);

		std::ostream& Print(std::ostream& out) const override;
		std::ostream& PrintShort(std::ostream& out) const;

		BranchPtr CreateBranch(const std::string& name);

		std::string Name;
		FunctionTypePtr Type;
		bool IsConstructor;
		std::vector<ArgPtr> Args;
		TypePtr Callee;

		std::vector<BranchPtr> Branches;
	};

	struct Branch : Instruction
	{
		Branch(const std::string& name);
		bool HasTerminator() const;

		std::ostream& Print(std::ostream& out) const override;

		std::string Name;
	};

	struct CreateVarInst : Instruction
	{
		CreateVarInst(const std::string& name, TypePtr type, ValuePtr value);

		std::ostream& Print(std::ostream& out) const override;

		std::string Name;
		TypePtr Type;
		ValuePtr Value;
	};

	struct RetInst : Instruction
	{
		RetInst(ValuePtr value);

		bool IsTerminator() const override;

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Value;
	};

	typedef std::shared_ptr<struct Branch> BranchPtr;
	struct SplitInst : Instruction
	{
		SplitInst(ValuePtr condition, BranchPtr _true, BranchPtr _false);
		bool IsTerminator() const override;

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Condition;
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

	struct CallInst : Instruction
	{
		CallInst(FunctionPtr function, ValuePtr callee, const std::vector<ValuePtr>& args, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		FunctionPtr Function;
		ValuePtr Callee;
		std::vector<ValuePtr> Args;
		ValuePtr Result;
	};

	struct GetElementInst : Instruction
	{
		GetElementInst(ValuePtr thing, const std::string& element, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Thing;
		std::string Element;
		ValuePtr Result;
	};

	struct AssignVarInst : Instruction
	{
		AssignVarInst(ValuePtr var, ValuePtr value);

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Var;
		ValuePtr Value;
	};

	struct AddInst : Instruction
	{
		AddInst(MathInstMode mode, ValuePtr left, ValuePtr right, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		MathInstMode Mode;
		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct SubInst : Instruction
	{
		SubInst(MathInstMode mode, ValuePtr left, ValuePtr right, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		MathInstMode Mode;
		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct MulInst : Instruction
	{
		MulInst(ValuePtr left, ValuePtr right, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct DivInst : Instruction
	{
		DivInst(ValuePtr left, ValuePtr right, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct NegInst : Instruction
	{
		NegInst(ValuePtr value, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Value;
		ValuePtr Result;
	};

	struct CmpInst : Instruction
	{
		CmpInst(MathInstMode mode, CmpInstMode cmpmode, ValuePtr left, ValuePtr right, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		MathInstMode Mode;
		CmpInstMode CMPMode;
		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct SelInst : Instruction
	{
		SelInst(ValuePtr condition, ValuePtr _true, ValuePtr _false, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Condition;
		ValuePtr True;
		ValuePtr False;
		ValuePtr Result;
	};

	struct LAndInst : Instruction
	{
		LAndInst(ValuePtr left, ValuePtr right, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct ShLInst : Instruction
	{
		ShLInst(ValuePtr left, ValuePtr right, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct AndInst : Instruction
	{
		AndInst(ValuePtr left, ValuePtr right, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct OrInst : Instruction
	{
		OrInst(ValuePtr left, ValuePtr right, ValuePtr result);

		std::ostream& Print(std::ostream& out) const override;

		ValuePtr Left;
		ValuePtr Right;
		ValuePtr Result;
	};

	struct NativeInst : Instruction
	{
		NativeInst(NativeFunction function);

		std::ostream& Print(std::ostream& out) const override;

		NativeFunction Function;
	};
}
