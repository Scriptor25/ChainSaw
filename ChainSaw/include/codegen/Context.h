#pragma once

#include <codegen/Def.h>

#include <filesystem>
#include <map>
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
		ValueRefPtr CreateVariable(const std::string& name, TypePtr type);
		ValueRefPtr GetVariable(const std::string& name);
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

		void CreateSplit(ValueRefPtr condition, BranchPtr _true, BranchPtr _false);
		void CreateFlow(BranchPtr branch);
		void CreateVar(const std::string& name, TypePtr type, ValueRefPtr value = nullptr);
		void CreateEmptyRet();
		void CreateRet(ValueRefPtr value);

		ValueRefPtr CreateCall(FunctionPtr function, ValueRefPtr callee, const std::vector<ValueRefPtr>& args);
		ValueRefPtr CreateGetElement(ValueRefPtr thing, const std::string& element);
		ValueRefPtr CreateAssign(ValueRefPtr var, ValueRefPtr value);

		ValueRefPtr CreateCmpLT_NN(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateCmpGT_NN(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateCmpLE_NN(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateCmpEQ_NN(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateCmpNE_SS(ValueRefPtr left, ValueRefPtr right);

		ValueRefPtr CreateAddNN(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateAddCC(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateAddSS(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateAddNS(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateAddCS(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateAddSN(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateAddSC(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateAddCN(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateAddNC(ValueRefPtr left, ValueRefPtr right);

		ValueRefPtr CreateSubNN(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateSubCC(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateSubCN(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateSubNC(ValueRefPtr left, ValueRefPtr right);

		ValueRefPtr CreateMul(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateDiv(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateRem(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateAnd(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateOr(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateXOr(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateLAnd(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateLOr(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateShL(ValueRefPtr left, ValueRefPtr right);
		ValueRefPtr CreateShR(ValueRefPtr left, ValueRefPtr right);

		ValueRefPtr CreateNeg(ValueRefPtr value);
		ValueRefPtr CreateNot(ValueRefPtr value);
		ValueRefPtr CreateInv(ValueRefPtr value);

		ValueRefPtr CreateSel(ValueRefPtr condition, TypePtr type, ValueRefPtr _true, ValueRefPtr _false);

	private:
		std::map<std::string, TypePtr> m_Types;
		std::map<TypePtr, std::map<std::string, std::map<FunctionTypePtr, FunctionPtr>>> m_Functions;

		std::map<std::string, ValueRefPtr> m_GlobalVariables;
		std::map<std::string, ValueRefPtr> m_Variables;

		std::vector<std::filesystem::path> m_Filepaths;

		FunctionPtr m_GlobalInsertPoint;
		InstructionPtr m_InsertPoint;
	};
}
