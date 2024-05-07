#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/FunctionRef.hpp>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/Host.h>
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

    // m_JIT = llvm::ExitOnError()(CSawJIT::Create());

    m_Context = std::make_unique<llvm::LLVMContext>();
    m_Builder = std::make_unique<llvm::IRBuilder<>>(*m_Context);
    m_Module = std::make_unique<llvm::Module>(moduleName, *m_Context);
    // m_Module->setDataLayout(m_JIT->GetDataLayout());

    const auto triple = llvm::sys::getDefaultTargetTriple();
    m_Module->setTargetTriple(triple);

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
}

llvm::LLVMContext& csaw::Builder::GetContext()
{
    return *m_Context;
}

llvm::IRBuilder<>& csaw::Builder::GetBuilder()
{
    return *m_Builder;
}

llvm::Module& csaw::Builder::GetModule()
{
    return *m_Module;
}

int csaw::Builder::Main(const int argc, char** argv)
{
    // llvm::ExitOnError()(m_JIT->AddModule(m_Module));

    // const auto main_fn = llvm::ExitOnError()(m_JIT->Lookup("main")).getAddress().toPtr<int(*)(int, char**)>();
    // return main_fn(argc, argv);
    return 0;
}

const csaw::FunctionRef& csaw::Builder::GetFunction(const std::string& name, const TypePtr& callee, const std::vector<TypePtr>& args)
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

        return ref;
    }

    return {};
}

csaw::FunctionRef& csaw::Builder::GetOrCreateFunction(const std::string& name, const bool constructor, const TypePtr& callee, const std::vector<TypePtr>& args, const bool vararg, const TypePtr& result)
{
    for (auto& ref : m_Functions[name])
    {
        if (name != ref.Name) continue;
        if (constructor != ref.IsConstructor) continue;
        if (callee != ref.Callee) continue;
        if (args.size() != ref.Args.size()) continue;
        if (vararg != ref.IsVarArg) continue;
        if (result != ref.Result) continue;

        size_t i = 0;
        for (; i < ref.Args.size(); ++i)
            if (ref.Args[i] != args[i]) break;
        if (i < ref.Args.size()) continue;

        return ref;
    }

    return m_Functions[name].emplace_back(nullptr, name, constructor, callee, args, vararg, result);
}

bool csaw::Builder::IsGlobal() const
{
    return !m_Builder->GetInsertBlock();
}

std::pair<csaw::ValueRef, csaw::ValueRef> csaw::Builder::CastToBestOf(const ValueRef& left, const ValueRef& right)
{
    if (left.Type() == right.Type())
        return {left, right};

    if (left.Type()->isIntegerTy() && right.Type()->isIntegerTy())
    {
        if (left.Type()->getIntegerBitWidth() > right.Type()->getIntegerBitWidth())
        {
            const auto result = m_Builder->CreateIntCast(right.Load(*this), left.Type(), true);
            return {left, {*this, ValueRefMode_Constant, result, left.RawType()}};
        }

        const auto result = m_Builder->CreateIntCast(left.Load(*this), right.Type(), true);
        return {{*this, ValueRefMode_Constant, result, right.RawType()}, right};
    }

    if (left.Type()->isIntegerTy() && right.Type()->isFloatingPointTy())
    {
        const auto result = m_Builder->CreateSIToFP(left.Load(*this), right.Type());
        return {{*this, ValueRefMode_Constant, result, right.RawType()}, right};
    }

    throw std::runtime_error("not yet implemented");
}
