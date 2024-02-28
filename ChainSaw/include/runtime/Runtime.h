#pragma once

#include <codegen/Def.h>

#include <map>
#include <string>

namespace csaw::codegen
{
	typedef std::shared_ptr<struct NativeInst> NativeInstPtr;
}

namespace csaw::runtime
{
	using namespace csaw::codegen;

	typedef std::shared_ptr<class Runtime> RuntimePtr;

	class Runtime
		: public std::enable_shared_from_this<Runtime>
	{
	public:
		static void RegisterNativeFunc(const std::string& name, FunctionTypePtr type, NativeInstPtr ptr);

	private:
		static std::map<FunctionTypePtr, std::map<std::string, NativeInstPtr>> NATIVE_FUNCTIONS;

	public:
		Runtime(ContextPtr context);
		Runtime(RuntimePtr runtime);
		Runtime(ContextPtr context, RuntimePtr runtime);

		ContextPtr Context();

		bool PreStart();
		ValueRefPtr Call(const std::string& name, ValueRefPtr callee, const std::vector<ValueRefPtr>& args);

	private:
		RuntimePtr GetGlobal();

		ValueRefPtr Create(const std::string& name, ValueRefPtr ref);
		ValueRefPtr Get(const std::string& name);
		ValueRefPtr Set(const std::string& name, ValueRefPtr ref);
		ValueRefPtr& GetRef(const std::string& name);

		ValueRefPtr Call(RuntimePtr runtime, FunctionPtr function, ValueRefPtr callee, const std::vector<ValueRefPtr>& args);

		InstructionPtr Evaluate(InstructionPtr ptr);

		InstructionPtr Evaluate(CreateVarInstPtr ptr);
		InstructionPtr Evaluate(RetInstPtr ptr);
		InstructionPtr Evaluate(SplitInstPtr ptr);
		InstructionPtr Evaluate(FlowInstPtr ptr);

		InstructionPtr Evaluate(CallInstPtr ptr);
		InstructionPtr Evaluate(GetElementInstPtr ptr);
		InstructionPtr Evaluate(AssignVarInstPtr ptr);
		InstructionPtr Evaluate(AddInstPtr ptr);
		InstructionPtr Evaluate(SubInstPtr ptr);
		InstructionPtr Evaluate(MulInstPtr ptr);
		InstructionPtr Evaluate(DivInstPtr ptr);
		InstructionPtr Evaluate(NegInstPtr ptr);
		InstructionPtr Evaluate(CmpInstPtr ptr);
		InstructionPtr Evaluate(SelInstPtr ptr);
		InstructionPtr Evaluate(LAndInstPtr ptr);
		InstructionPtr Evaluate(ShLInstPtr ptr);
		InstructionPtr Evaluate(AndInstPtr ptr);
		InstructionPtr Evaluate(OrInstPtr ptr);

		InstructionPtr Evaluate(NativeInstPtr ptr);

	private:
		ContextPtr m_Context;
		RuntimePtr m_Parent;

		std::map<std::string, ValueRefPtr> m_References;

		ValueRefPtr m_Callee;
		std::vector<ValueRefPtr> m_Args;
		ValueRefPtr m_Result;
	};
}
