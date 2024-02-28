#include <codegen/Context.h>
#include <codegen/Function.h>
#include <codegen/Instruction.h>
#include <codegen/Native.h>
#include <runtime/Runtime.h>
#include <codegen/Type.h>
#include <codegen/Value.h>

#include <iostream>

using namespace csaw::codegen;

std::map<FunctionTypePtr, std::map<std::string, NativeInstPtr>> csaw::runtime::Runtime::NATIVE_FUNCTIONS;

void csaw::runtime::Runtime::RegisterNativeFunc(const std::string& name, FunctionTypePtr type, NativeInstPtr ptr)
{
	NATIVE_FUNCTIONS[type][name] = ptr;
}

csaw::runtime::Runtime::Runtime(ContextPtr context)
	: Runtime(context, nullptr)
{
}

csaw::runtime::Runtime::Runtime(RuntimePtr runtime)
	: Runtime(runtime->m_Context, runtime)
{
}

csaw::runtime::Runtime::Runtime(ContextPtr context, RuntimePtr runtime)
	: m_Context(context), m_Parent(runtime)
{
}

ContextPtr csaw::runtime::Runtime::Context()
{
	return m_Context;
}

bool csaw::runtime::Runtime::PreStart()
{
	size_t missing = 0;
	for (auto& function : m_Context->Functions())
	{
		if (!function->Next)
		{
			if (auto& inst = NATIVE_FUNCTIONS[function->Type][function->Name])
				function->Next = inst;
			else
			{
				std::cerr << "missing function '" << function->Name << "' " << function->Type->GetName() << std::endl;
				missing++;
			}
		}
	}

	if (missing)
	{
		std::cerr << missing << " missing functions" << std::endl;
		return false;
	}

	auto& global = m_Context->GetGlobal();
	Call(shared_from_this(), global, nullptr, {});

	return true;
}

ValueRefPtr csaw::runtime::Runtime::Call(const std::string& name, ValueRefPtr callee, const std::vector<ValueRefPtr>& args)
{
	std::vector<csaw::codegen::TypePtr> argtypes;
	for (auto& arg : args)
		argtypes.push_back(arg->Type());

	auto runtime = std::make_shared<Runtime>(GetGlobal());
	auto function = m_Context->GetFunction(name, callee->Type(m_Context->GetEmptyType()), argtypes);
	return Call(runtime, function, callee, args);
}

csaw::runtime::RuntimePtr csaw::runtime::Runtime::GetGlobal()
{
	if (m_Parent)
		return m_Parent->GetGlobal();
	return shared_from_this();
}

ValueRefPtr csaw::runtime::Runtime::Create(const std::string& name, ValueRefPtr ref)
{
	if (!ref) throw;
	return m_References[name] = ref;
}

ValueRefPtr csaw::runtime::Runtime::Get(const std::string& name)
{
	auto& entry = GetRef(name);
	if (!entry) throw;
	return entry;
}

ValueRefPtr csaw::runtime::Runtime::Set(const std::string& name, ValueRefPtr ref)
{
	if (!ref) throw;
	auto& entry = GetRef(name);
	if (!entry) throw;
	return entry = ref;
}

ValueRefPtr& csaw::runtime::Runtime::GetRef(const std::string& name)
{
	if (auto& entry = m_References[name]) return entry;
	if (!m_Parent) return m_References[name];
	return m_Parent->GetRef(name);
}

ValueRefPtr csaw::runtime::Runtime::Call(RuntimePtr runtime, FunctionPtr function, ValueRefPtr callee, const std::vector<ValueRefPtr>& args)
{
	runtime->m_Callee = callee;
	runtime->m_Args = args;

	for (size_t i = 0; i < function->Args.size(); i++)
		runtime->Create(function->Args[i]->Name, args[i]);

	InstructionPtr ptr = function->Next;
	while (ptr)
		ptr = runtime->Evaluate(ptr);

	return runtime->m_Result;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(InstructionPtr ptr)
{
	if (auto p = std::dynamic_pointer_cast<CreateVarInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<RetInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<SplitInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<FlowInst>(ptr))
		return Evaluate(p);

	if (auto p = std::dynamic_pointer_cast<CallInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<GetElementInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<AssignVarInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<AddInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<SubInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<MulInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<DivInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<NegInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<CmpInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<SelInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<LAndInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<ShLInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<AndInst>(ptr))
		return Evaluate(p);
	if (auto p = std::dynamic_pointer_cast<OrInst>(ptr))
		return Evaluate(p);

	if (auto p = std::dynamic_pointer_cast<NativeInst>(ptr))
		return Evaluate(p);

	throw;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(CreateVarInstPtr ptr)
{
	Create(ptr->Name, ptr->Value);
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(RetInstPtr ptr)
{
	m_Result->Value = ptr->Value->Value;
	return InstructionPtr();
}

InstructionPtr csaw::runtime::Runtime::Evaluate(SplitInstPtr ptr)
{
	auto condition = ptr->Condition->Value->AsConst()->AsNum();
	if (condition->Value) return ptr->True->Next;
	return ptr->False->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(FlowInstPtr ptr)
{
	return ptr->Branch->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(CallInstPtr ptr)
{
	auto runtime = std::make_shared<Runtime>(GetGlobal());
	ptr->Result = Call(runtime, ptr->Function, ptr->Callee, ptr->Args);
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(GetElementInstPtr ptr) // TODO
{
	auto thing = ptr->Thing->Value->AsConst()->AsThing();
	ptr->Result->Value = thing->Elements[ptr->Element];
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(AssignVarInstPtr ptr)
{
	ptr->Var = ptr->Value;
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(AddInstPtr ptr)
{
	auto left = ptr->Left->Value->AsConst();
	auto right = ptr->Right->Value->AsConst();

	ValuePtr value;
	switch (ptr->Mode)
	{
	case MODE_NN:
		value = m_Context->GetConstNum(left->AsNum()->Value + right->AsNum()->Value);
		break;
	case MODE_CC:
		value = m_Context->GetConstChr(left->AsChr()->Value + right->AsChr()->Value);
		break;
	case MODE_SS:
		value = m_Context->GetConstStr(left->AsStr()->Value + right->AsStr()->Value);
		break;
	case MODE_NS:
		value = m_Context->GetConstStr(std::to_string(left->AsNum()->Value) + right->AsStr()->Value);
		break;
	case MODE_CS:
		value = m_Context->GetConstStr(left->AsChr()->Value + right->AsStr()->Value);
		break;
	case MODE_SN:
		value = m_Context->GetConstStr(left->AsStr()->Value + std::to_string(right->AsNum()->Value));
		break;
	case MODE_SC:
		value = m_Context->GetConstStr(left->AsStr()->Value + right->AsChr()->Value);
		break;
	case MODE_CN:
		value = m_Context->GetConstChr(left->AsChr()->Value + char(right->AsNum()->Value));
		break;
	case MODE_NC:
		value = m_Context->GetConstNum(left->AsNum()->Value + right->AsChr()->Value);
		break;

	default:
		throw;
	}

	ptr->Result->Value = value;
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(SubInstPtr ptr)
{
	auto left = ptr->Left->Value->AsConst();
	auto right = ptr->Right->Value->AsConst();

	ValuePtr value;
	switch (ptr->Mode)
	{
	case MODE_NN:
		value = m_Context->GetConstNum(left->AsNum()->Value - right->AsNum()->Value);
		break;
	case MODE_CC:
		value = m_Context->GetConstChr(left->AsChr()->Value - right->AsChr()->Value);
		break;
	case MODE_CN:
		value = m_Context->GetConstChr(left->AsChr()->Value - char(right->AsNum()->Value));
		break;
	case MODE_NC:
		value = m_Context->GetConstNum(left->AsNum()->Value - right->AsChr()->Value);
		break;

	default:
		throw;
	}

	ptr->Result->Value = value;
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(MulInstPtr ptr)
{
	auto left = ptr->Left->Value->AsConst()->AsNum();
	auto right = ptr->Right->Value->AsConst()->AsNum();

	ptr->Result->Value = m_Context->GetConstNum(left->Value * right->Value);
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(DivInstPtr ptr)
{
	auto left = ptr->Left->Value->AsConst()->AsNum();
	auto right = ptr->Right->Value->AsConst()->AsNum();

	ptr->Result->Value = m_Context->GetConstNum(left->Value / right->Value);
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(NegInstPtr ptr)
{
	auto value = ptr->Value->Value->AsConst()->AsNum();
	ptr->Result->Value = m_Context->GetConstNum(-value->Value);
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(CmpInstPtr ptr)
{
	auto left = ptr->Left->Value->AsConst();
	auto right = ptr->Right->Value->AsConst();

	bool value;
	switch (ptr->Mode)
	{
	case MODE_NN:
		switch (ptr->CMPMode)
		{
		case CMP_LT:
			value = left->AsNum()->Value < right->AsNum()->Value;
			break;
		case CMP_GT:
			value = left->AsNum()->Value > right->AsNum()->Value;
			break;
		case CMP_LE:
			value = left->AsNum()->Value <= right->AsNum()->Value;
			break;
		case CMP_GE:
			value = left->AsNum()->Value >= right->AsNum()->Value;
			break;
		case CMP_EQ:
			value = left->AsNum()->Value == right->AsNum()->Value;
			break;
		case CMP_NE:
			value = left->AsNum()->Value != right->AsNum()->Value;
			break;

		default:
			throw;
		}
		break;
	case MODE_SS:
		switch (ptr->CMPMode)
		{
		case CMP_LT:
			value = left->AsStr()->Value < right->AsStr()->Value;
			break;
		case CMP_GT:
			value = left->AsStr()->Value > right->AsStr()->Value;
			break;
		case CMP_LE:
			value = left->AsStr()->Value <= right->AsStr()->Value;
			break;
		case CMP_GE:
			value = left->AsStr()->Value >= right->AsStr()->Value;
			break;
		case CMP_EQ:
			value = left->AsStr()->Value == right->AsStr()->Value;
			break;
		case CMP_NE:
			value = left->AsStr()->Value != right->AsStr()->Value;
			break;

		default:
			throw;
		}
		break;

	default:
		throw;
	}

	ptr->Result->Value = m_Context->GetConstNum(value ? 1.0 : 0.0);
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(SelInstPtr ptr)
{
	auto condition = ptr->Condition->Value->AsConst()->AsNum();

	ptr->Result =
		condition->Value
		? ptr->True
		: ptr->False;
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(LAndInstPtr ptr)
{
	auto left = ptr->Left->Value->AsConst()->AsNum();
	auto right = ptr->Right->Value->AsConst()->AsNum();

	ptr->Result->Value = m_Context->GetConstNum(left->Value && right->Value ? 1.0 : 0.0);
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(ShLInstPtr ptr)
{
	auto left = ptr->Left->Value->AsConst()->AsNum();
	auto right = ptr->Right->Value->AsConst()->AsNum();

	ptr->Result->Value = m_Context->GetConstNum(long(left->Value) << long(right->Value));
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(AndInstPtr ptr)
{
	auto left = ptr->Left->Value->AsConst()->AsNum();
	auto right = ptr->Right->Value->AsConst()->AsNum();

	ptr->Result->Value = m_Context->GetConstNum(long(left->Value) & long(right->Value));
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(OrInstPtr ptr)
{
	auto left = ptr->Left->Value->AsConst()->AsNum();
	auto right = ptr->Right->Value->AsConst()->AsNum();

	ptr->Result->Value = m_Context->GetConstNum(long(left->Value) | long(right->Value));
	return ptr->Next;
}

InstructionPtr csaw::runtime::Runtime::Evaluate(NativeInstPtr ptr)
{
	m_Result = ptr->Function(m_Context, m_Callee, m_Args);
	return ptr->Next;
}
