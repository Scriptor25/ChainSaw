#pragma once

#include <codegen/Function.h>
#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace csaw::codegen
{
	class Context
	{
	public:
		Context();

		TypePtr GetType(const std::string& name);
		NumTypePtr GetNumType();
		ChrTypePtr GetChrType();
		StrTypePtr GetStrType();
		EmptyTypePtr GetEmptyType();
		ThingTypePtr GetThingType(const std::string& name);
		ThingTypePtr CreateThingType(const std::string& name, const std::map<std::string, TypePtr>& elements);
		FunctionTypePtr GetFunctionType(TypePtr result, const std::vector<TypePtr>& argtypes, bool isvararg);

		FunctionPtr GetFunction(const std::string& name, TypePtr callee, FunctionTypePtr type, bool isconstructor);
		FunctionPtr GetFunction(const std::string& name, TypePtr callee, const std::vector<TypePtr>& argtypes);

		std::string& Filepath();
		const std::string& Filepath() const;

		const std::map<std::string, TypePtr>& Types() const;
		const std::map<TypePtr, std::map<std::string, std::map<FunctionTypePtr, FunctionPtr>>>& Functions() const;
		const std::vector<std::string>& Filepaths() const;

		const std::vector<FunctionPtr> ListFunctions() const;
		const FunctionPtr& GetGlobal() const;

		void PushFilepath(const std::string& filepath);
		void PopFilepath();

		void ClearVariables();
		void CreateVariable(const std::string& name, TypePtr type);
		void SetVariable(const std::string& name, TypePtr type);
		TypePtr GetVariable(const std::string& name);

		ConstNumPtr GetConstNum(double value);
		ConstStrPtr GetConstStr(const std::string& value);

		void SetInsertPoint(InstructionPtr insertpoint);
		void SetInsertGlobal();

		FunctionPtr GetCurrentFunction();

		BranchPtr CreateBranch();
		void CreateSplit(ValuePtr condition, BranchPtr _true, BranchPtr _false);
		void CreateFlow(BranchPtr branch);

		void CreateVar(const std::string& name, TypePtr type, ValuePtr value);
		void CreateEmptyRet();
		void CreateRet(ValuePtr value);

		ValuePtr CreateCall(FunctionPtr function, ValuePtr callee, const std::vector<ValuePtr>& args);
		ValuePtr CreateGetElement(ValuePtr thing, const std::string& element);
		ValuePtr CreateGetVar(const std::string& name);
		ValuePtr CreateAssign(ValuePtr var, ValuePtr value);

		ValuePtr CreateCmpLT_NN(ValuePtr left, ValuePtr right);
		ValuePtr CreateCmpLE_NN(ValuePtr left, ValuePtr right);
		ValuePtr CreateCmpEQ_NN(ValuePtr left, ValuePtr right);
		ValuePtr CreateCmpNE_SS(ValuePtr left, ValuePtr right);

		ValuePtr CreateAddNN(ValuePtr left, ValuePtr right);
		ValuePtr CreateAddCC(ValuePtr left, ValuePtr right);
		ValuePtr CreateAddSS(ValuePtr left, ValuePtr right);
		ValuePtr CreateAddNS(ValuePtr left, ValuePtr right);
		ValuePtr CreateAddCS(ValuePtr left, ValuePtr right);
		ValuePtr CreateAddSN(ValuePtr left, ValuePtr right);
		ValuePtr CreateAddSC(ValuePtr left, ValuePtr right);
		ValuePtr CreateAddCN(ValuePtr left, ValuePtr right);
		ValuePtr CreateAddNC(ValuePtr left, ValuePtr right);

		ValuePtr CreateSubNN(ValuePtr left, ValuePtr right);
		ValuePtr CreateSubCC(ValuePtr left, ValuePtr right);
		ValuePtr CreateSubCN(ValuePtr left, ValuePtr right);
		ValuePtr CreateSubNC(ValuePtr left, ValuePtr right);

		ValuePtr CreateMul(ValuePtr left, ValuePtr right);
		ValuePtr CreateDiv(ValuePtr left, ValuePtr right);
		ValuePtr CreateRem(ValuePtr left, ValuePtr right);
		ValuePtr CreateAnd(ValuePtr left, ValuePtr right);
		ValuePtr CreateOr(ValuePtr left, ValuePtr right);
		ValuePtr CreateXOr(ValuePtr left, ValuePtr right);
		ValuePtr CreateLAnd(ValuePtr left, ValuePtr right);
		ValuePtr CreateLOr(ValuePtr left, ValuePtr right);
		ValuePtr CreateShL(ValuePtr left, ValuePtr right);
		ValuePtr CreateShR(ValuePtr left, ValuePtr right);

		ValuePtr CreateNeg(ValuePtr value);
		ValuePtr CreateNot(ValuePtr value);
		ValuePtr CreateInv(ValuePtr value);

		ValuePtr CreateSel(ValuePtr condition, TypePtr type, ValuePtr _true, ValuePtr _false);

	private:
		std::map<std::string, TypePtr> m_Types;
		std::map<TypePtr, std::map<std::string, std::map<FunctionTypePtr, FunctionPtr>>> m_Functions;

		std::map<std::string, TypePtr> m_GlobalVariables;
		std::map<std::string, TypePtr> m_Variables;

		std::vector<std::string> m_Filepath;

		FunctionPtr m_GlobalInsertPoint;
		InstructionPtr m_InsertPoint;
	};

	typedef std::shared_ptr<Context> ContextPtr;

	std::ostream& operator<<(std::ostream& out, const Context& context);
}
