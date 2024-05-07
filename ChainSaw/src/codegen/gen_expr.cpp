#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/FunctionRef.hpp>
#include <csaw/codegen/ValueRef.hpp>
#include <csaw/lang/Expr.hpp>

csaw::ValueRef csaw::Builder::Gen(const ExpressionPtr& ptr)
{
    if (const auto p = std::dynamic_pointer_cast<BinaryExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<CallExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<CastExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<CharExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<FloatExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<IdentifierExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<IndexExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<IntExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<MemberExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<ReferenceExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<SelectExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<StringExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<UnaryExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<VarArgExpression>(ptr))
        return Gen(*p);

    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::Gen(const BinaryExpression& expression)
{
    auto left = Gen(expression.Left);
    auto right = Gen(expression.Right);
    auto op = expression.Operator;

    if (const auto& functionref = GetFunction(op, nullptr, {left.RawType(), right.RawType()}); functionref.Function)
    {
        const auto result = m_Builder->CreateCall(functionref.Function->getFunctionType(), functionref.Function, {left.Load(*this), right.Load(*this)});
        return {*this, ValueRefMode_Constant, result, functionref.Result};
    }

    if (const auto& functionref = GetFunction(op, left.RawType(), {right.RawType()}); functionref.Function)
    {
        const auto result = m_Builder->CreateCall(functionref.Function->getFunctionType(), functionref.Function, {left.Load(*this), right.Load(*this)});
        return {*this, ValueRefMode_Constant, result, functionref.Result};
    }

    if (op == "=")
    {
        left.Store(*this, right.Load(*this));
        return left;
    }

    const auto [lhs, rhs] = CastToBestOf(left, right);

    if (op == "==") return GenCmpEQ(lhs, rhs);
    if (op == "!=") return GenCmpNE(lhs, rhs);
    if (op == "<=") return GenCmpLE(lhs, rhs);
    if (op == ">=") return GenCmpGE(lhs, rhs);

    const auto equ_idx = op.find('=');
    const auto assign = equ_idx != std::string::npos;
    if (assign) op = op.substr(0, equ_idx);

    ValueRef value;
    if (op == "&") value = GenAnd(lhs, rhs);
    if (op == "&&") value = GenLogicalAnd(lhs, rhs);
    if (op == "|") value = GenOr(lhs, rhs);
    if (op == "||") value = GenLogicalOr(lhs, rhs);
    if (op == "^") value = GenXor(lhs, rhs);
    if (op == "<") value = GenCmpLT(lhs, rhs);
    if (op == ">") value = GenCmpGT(lhs, rhs);
    if (op == "<<") value = GenShl(lhs, rhs);
    if (op == ">>") value = GenAShr(lhs, rhs);
    if (op == ">>>") value = GenLShr(lhs, rhs);
    if (op == "+") value = GenAdd(lhs, rhs);
    if (op == "-") value = GenSub(lhs, rhs);
    if (op == "*") value = GenMul(lhs, rhs);
    if (op == "/") value = GenDiv(lhs, rhs);
    if (op == "%") value = GenRem(lhs, rhs);

    if (value.Mode() == ValueRefMode_Invalid)
        throw std::runtime_error("unhandled binary operator");

    if (assign)
    {
        left.Store(*this, value.Load(*this));
        return left;
    }

    return value;
}

csaw::ValueRef csaw::Builder::Gen(const CallExpression& expression)
{
    std::string name;
    ExpressionPtr callee = nullptr;
    if (const auto member_expression = std::dynamic_pointer_cast<MemberExpression>(expression.Callee))
    {
        name = member_expression->Member;
        callee = member_expression->Object;
    }
    else if (const auto identifier_expression = std::dynamic_pointer_cast<IdentifierExpression>(expression.Callee))
        name = identifier_expression->Id;
    else throw std::runtime_error("wtf have you done");

    std::vector<TypePtr> argtypes(expression.Args.size());
    std::vector<llvm::Value*> args(expression.Args.size());
    for (size_t i = 0; i < args.size(); ++i)
    {
        const auto ref = Gen(expression.Args[i]);
        argtypes[i] = ref.RawType();
        args[i] = ref.Load(*this);
    }

    const auto calleeref = callee ? Gen(callee) : ValueRef();
    if (callee) args.insert(args.begin(), calleeref.Load(*this));

    const auto& functionref = GetFunction(name, callee ? calleeref.RawType() : nullptr, argtypes);
    if (!functionref.Function)
        throw std::runtime_error("undefined function");

    if (functionref.IsConstructor)
        args.insert(args.begin(), m_Builder->CreateAlloca(Gen(functionref.Result)));

    const auto result = m_Builder->CreateCall(functionref.Function->getFunctionType(), functionref.Function, args);
    return {*this, ValueRefMode_Constant, result, functionref.Result};
}

csaw::ValueRef csaw::Builder::Gen(const CastExpression& expression)
{
    const auto valueref = Gen(expression.Value);
    const auto value = valueref.Load(*this);
    const auto type = Gen(expression.Type);

    llvm::Value* result = nullptr;
    if (valueref.Type()->isIntegerTy())
    {
        if (type->isIntegerTy())
            result = m_Builder->CreateIntCast(value, type, true);
        else if (type->isFloatingPointTy())
            result = m_Builder->CreateSIToFP(value, type);
        else if (type->isPointerTy())
            result = m_Builder->CreateIntToPtr(value, type);
    }
    else if (valueref.Type()->isFloatingPointTy())
    {
        if (type->isFloatingPointTy())
            result = m_Builder->CreateFPCast(value, type);
        else if (type->isIntegerTy())
            result = m_Builder->CreateFPToSI(value, type);
    }
    else if (valueref.Type()->isPointerTy())
    {
        if (type->isPointerTy())
            result = m_Builder->CreatePointerCast(value, type);
        else if (type->isIntegerTy())
            result = m_Builder->CreatePtrToInt(value, type);
    }

    if (!result)
        throw std::runtime_error("not yet implemented");

    return {*this, ValueRefMode_Constant, result, expression.Type};
}

csaw::ValueRef csaw::Builder::Gen(const CharExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::Gen(const FloatExpression& expression)
{
    const auto result = llvm::ConstantFP::get(m_Builder->getDoubleTy(), expression.Value);
    return {*this, ValueRefMode_Constant, result, Type::Get("flt64")};
}

csaw::ValueRef csaw::Builder::Gen(const IdentifierExpression& expression)
{
    if (const auto& value = m_Values[expression.Id]; value.Mode() != ValueRefMode_Invalid)
        return value;
    return m_GlobalValues[expression.Id];
}

csaw::ValueRef csaw::Builder::Gen(const IndexExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::Gen(const IntExpression& expression)
{
    const auto result = m_Builder->getInt32(expression.Value);
    return {*this, ValueRefMode_Constant, result, Type::Get("int32")};
}

csaw::ValueRef csaw::Builder::Gen(const MemberExpression& expression)
{
    const auto object = Gen(expression.Object);
    const auto member = expression.Member;

    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::Gen(const ReferenceExpression& expression)
{
    auto value = Gen(expression.Value);
    return value.GetReference(*this);
}

csaw::ValueRef csaw::Builder::Gen(const SelectExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

csaw::ValueRef csaw::Builder::Gen(const StringExpression& expression)
{
    const auto result = m_Builder->CreateGlobalStringPtr(expression.Value);
    return {*this, ValueRefMode_Pointer, result, PointerType::Get(Type::Get("int8"))};
}

csaw::ValueRef csaw::Builder::Gen(const UnaryExpression& expression)
{
    const auto value = Gen(expression.Value);
    const auto op = expression.Operator;

    if (expression.OpRight)
    {
        if (const auto& functionref = GetFunction(op, nullptr, {value.RawType()}); functionref.Function)
        {
            const auto result = m_Builder->CreateCall(functionref.Function->getFunctionType(), functionref.Function, {value.Load(*this)});
            return {*this, ValueRefMode_Constant, result, functionref.Result};
        }
    }
    else if (const auto& functionref = GetFunction(op, value.RawType(), {}); functionref.Function)
    {
        const auto result = m_Builder->CreateCall(functionref.Function->getFunctionType(), functionref.Function, {value.Load(*this)});
        return {*this, ValueRefMode_Constant, result, functionref.Result};
    }

    ValueRef result;
    if (op == "-") result = GenNeg(value);
    if (op == "!") result = GenNot(value);
    if (op == "~") result = GenInv(value);
    if (op == "++") result = GenInc(value, expression.OpRight);
    if (op == "--") result = GenDec(value, expression.OpRight);

    if (result.Mode() == ValueRefMode_Invalid)
        throw std::runtime_error("not yet implemented");

    return result;
}

csaw::ValueRef csaw::Builder::Gen(const VarArgExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}
