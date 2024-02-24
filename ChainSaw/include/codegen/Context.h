#pragma once

#include <codegen/Function.h>
#include <codegen/Instruction.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

#include <filesystem>
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

		std::ostream& Print() const;
		std::ostream& Print(std::ostream& out) const;

		TypePtr GetType(const std::string& name);
		NumTypePtr GetNumType();
		ChrTypePtr GetChrType();
		StrTypePtr GetStrType();
		EmptyTypePtr GetEmptyType();
		ThingTypePtr GetThingType(const std::string& name);
		FunctionTypePtr GetFunctionType(TypePtr result, const std::vector<TypePtr>& argtypes, bool isvararg);

		FunctionPtr GetFunction(const std::string& name, TypePtr callee, FunctionTypePtr type, bool isconstructor);
		FunctionPtr GetFunction(const std::string& name, TypePtr callee, const std::vector<TypePtr>& argtypes);

		std::filesystem::path& Filepath();
		const std::filesystem::path& Filepath() const;
		const std::vector<std::filesystem::path>& Filepaths() const;

		const std::vector<FunctionPtr> Functions() const;
		const std::vector<FunctionPtr> DefinedFunctions() const;
		const std::vector<FunctionPtr> DeclaredFunctions() const;
		const FunctionPtr& GetGlobal() const;

		void PushFilepath(const std::filesystem::path& filepath);
		void PopFilepath();

		void ClearVariables();
		ValuePtr CreateVariable(const std::string& name, TypePtr type);
		ValuePtr GetVariable(const std::string& name);
		void CreateArgs(const std::vector<ArgPtr>& args);

		ConstNumPtr GetConstNum(double value);
		ConstChrPtr GetConstChr(char value);
		ConstStrPtr GetConstStr(const std::string& value);
		ConstThingPtr GetConstThing(const std::string& type, const std::map<std::string, ValuePtr>& elements);

		void SetInsertPoint(InstructionPtr insertpoint);
		void SetInsertGlobal();
		bool IsInsertGlobal();
		InstructionPtr GetInsertPoint();
		BranchPtr GetInsertBranch();
		FunctionPtr GetInsertFunction();
		BranchPtr CreateBranch(const std::string& name = "");

		void CreateSplit(ValuePtr condition, BranchPtr _true, BranchPtr _false);
		void CreateFlow(BranchPtr branch);
		void CreateVar(const std::string& name, TypePtr type, ValuePtr value = nullptr);
		void CreateEmptyRet();
		void CreateRet(ValuePtr value);

		ValuePtr CreateCall(FunctionPtr function, ValuePtr callee, const std::vector<ValuePtr>& args);
		ValuePtr CreateGetElement(ValuePtr thing, const std::string& element);
		ValuePtr CreateAssign(ValuePtr var, ValuePtr value);

		ValuePtr CreateCmpLT_NN(ValuePtr left, ValuePtr right);
		ValuePtr CreateCmpGT_NN(ValuePtr left, ValuePtr right);
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

		std::map<std::string, ValuePtr> m_GlobalVariables;
		std::map<std::string, ValuePtr> m_Variables;

		std::vector<std::filesystem::path> m_Filepaths;

		FunctionPtr m_GlobalInsertPoint;
		InstructionPtr m_InsertPoint;
	};

	typedef std::shared_ptr<Context> ContextPtr;
}
