#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/FunctionRef.hpp>
#include <csaw/lang/Stmt.hpp>
#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

csaw::Builder::Builder(const std::string& moduleName)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    llvm::orc::LLJITBuilder builder;
    builder.setLinkProcessSymbolsByDefault(true);
    m_JIT = m_Error(builder.create());

    llvm::orc::SymbolMap symbols;
    symbols[m_JIT->mangleAndIntern("fprintf")] = {
        llvm::orc::ExecutorAddr(llvm::pointerToJITTargetAddress(&fprintf)),
        llvm::JITSymbolFlags()
    };
    m_Error(m_JIT->getMainJITDylib().define(absoluteSymbols(symbols)));

    m_Context = std::make_unique<llvm::LLVMContext>();
    m_Builder = std::make_unique<llvm::IRBuilder<>>(*m_Context);
    m_Module = std::make_unique<llvm::Module>(moduleName, *m_Context);
    m_Module->setDataLayout(m_JIT->getDataLayout());
    m_Module->setTargetTriple(m_JIT->getTargetTriple().str());

    m_FPM = std::make_unique<llvm::FunctionPassManager>();
    m_LAM = std::make_unique<llvm::LoopAnalysisManager>();
    m_FAM = std::make_unique<llvm::FunctionAnalysisManager>();
    m_CGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
    m_MAM = std::make_unique<llvm::ModuleAnalysisManager>();
    m_PIC = std::make_unique<llvm::PassInstrumentationCallbacks>();
    m_SI = std::make_unique<llvm::StandardInstrumentations>(*m_Context, true);
    m_SI->registerCallbacks(*m_PIC, m_MAM.get());

    m_FPM->addPass(llvm::InstCombinePass());
    m_FPM->addPass(llvm::ReassociatePass());
    m_FPM->addPass(llvm::GVNPass());
    m_FPM->addPass(llvm::SimplifyCFGPass());

    llvm::PassBuilder pb;
    pb.registerModuleAnalyses(*m_MAM);
    pb.registerFunctionAnalyses(*m_FAM);
    pb.crossRegisterProxies(*m_LAM, *m_FAM, *m_CGAM, *m_MAM);

    const auto function_type = llvm::FunctionType::get(m_Builder->getVoidTy(), false);
    m_GlobalParent = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage, "__global__", *m_Module);
    const auto entry_block = llvm::BasicBlock::Create(*m_Context, "entry", m_GlobalParent);
    m_Builder->SetInsertPoint(entry_block);
}

llvm::LLVMContext& csaw::Builder::GetContext() const
{
    return *m_Context;
}

llvm::IRBuilder<>& csaw::Builder::GetBuilder() const
{
    return *m_Builder;
}

llvm::Module& csaw::Builder::GetModule() const
{
    return *m_Module;
}

void csaw::Builder::Generate(const StatementPtr& ptr)
{
    Gen(ptr);
}

void csaw::Builder::Build() const
{
    m_Builder->CreateRetVoid();
    if (verifyFunction(*m_GlobalParent, &llvm::errs()))
    {
        m_GlobalParent->viewCFG();
        m_GlobalParent->eraseFromParent();
        CSAW_MESSAGE_NONE(true, "failed to verify __global__");
    }
    m_FPM->run(*m_GlobalParent, *m_FAM);
}

int csaw::Builder::Main(const int argc, const char** argv)
{
    m_Error(m_JIT->addIRModule(llvm::orc::ThreadSafeModule(std::move(m_Module), std::move(m_Context))));

    const auto global_fn = m_Error(m_JIT->lookup("__global__")).toPtr<void(*)()>();
    global_fn();

    const auto main_fn = m_Error(m_JIT->lookup("main")).toPtr<int(*)(int, const char**)>();
    return main_fn(argc, argv);
}

const csaw::FunctionRef* csaw::Builder::GetFunction(const std::string& name, const TypePtr& callee, const std::vector<TypePtr>& args)
{
    for (const auto& ref : m_Functions[name])
    {
        if (name != ref.Name) continue;
        if (callee != ref.Callee) continue;
        if (args.size() < ref.Args.size() || (args.size() > ref.Args.size() && !ref.IsVarArg)) continue;

        size_t i = 0;
        for (; i < ref.Args.size(); ++i)
            if (ref.Args[i] != args[i]) break;
        if (i < ref.Args.size()) continue;

        return &ref;
    }

    return nullptr;
}

csaw::FunctionRef& csaw::Builder::GetOrCreateFunction(const std::string& name, const TypePtr& callee, const TypePtr& result, const std::vector<TypePtr>& args, const bool isConstructor, const bool isVarArg)
{
    for (auto& ref : m_Functions[name])
    {
        if (name != ref.Name) continue;
        if (isConstructor != ref.IsConstructor) continue;
        if (callee != ref.Callee) continue;
        if (args.size() != ref.Args.size()) continue;
        if (isVarArg != ref.IsVarArg) continue;
        if (result != ref.Result) continue;

        size_t i = 0;
        for (; i < ref.Args.size(); ++i)
            if (ref.Args[i] != args[i]) break;
        if (i < ref.Args.size()) continue;

        return ref;
    }

    return m_Functions[name].emplace_back(nullptr, name, callee, result, args, isConstructor, isVarArg);
}

std::string csaw::Builder::FunctionSignatureString(const TypePtr& callee, const std::vector<TypePtr>& args)
{
    std::string signature;
    if (callee) signature += callee->Name;
    else signature += "<none>";
    signature += '(';
    for (size_t i = 0; i < args.size(); ++i)
    {
        if (i > 0) signature += ", ";
        signature += args[i]->Name;
    }
    signature += ')';
    return signature;
}

std::pair<int, csaw::TypePtr> csaw::Builder::ElementInStruct(const TypePtr& rawType, const std::string& element)
{
    if (!rawType->IsStruct())
        CSAW_MESSAGE_NONE(true, "type " + rawType->Name + " is not a struct");

    int i = 0;
    for (const auto& [name, type] : rawType->AsStruct()->Elements)
    {
        if (name == element)
            return {i, type};
        ++i;
    }

    CSAW_MESSAGE_NONE(true, rawType->Name + " does not have a member '" + element + "'");
}

std::pair<csaw::ValueRef, csaw::ValueRef> csaw::Builder::CastToBestOf(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType() == right.GetType())
        return {left, right};

    if (left.GetType()->isIntegerTy())
    {
        if (right.GetType()->isIntegerTy())
        {
            if (left.GetType()->getIntegerBitWidth() > right.GetType()->getIntegerBitWidth())
            {
                const auto value = m_Builder->CreateIntCast(right.GetValue(), left.GetType(), true);
                return {left, ValueRef::Constant(this, value, left.GetRawType())};
            }

            const auto value = m_Builder->CreateIntCast(left.GetValue(), right.GetType(), true);
            return {ValueRef::Constant(this, value, right.GetRawType()), right};
        }

        if (right.GetType()->isFloatingPointTy())
        {
            const auto value = m_Builder->CreateSIToFP(left.GetValue(), right.GetType());
            return {ValueRef::Constant(this, value, right.GetRawType()), right};
        }
    }

    if (left.GetType()->isFloatingPointTy())
    {
        if (right.GetType()->isIntegerTy())
        {
            const auto value = m_Builder->CreateSIToFP(right.GetValue(), left.GetType());
            return {left, ValueRef::Constant(this, value, left.GetRawType())};
        }
    }

    CSAW_MESSAGE_NONE(true, "superior cast between " + left.GetRawType()->Name + " and " + right.GetRawType()->Name + " is not implemented");
}
