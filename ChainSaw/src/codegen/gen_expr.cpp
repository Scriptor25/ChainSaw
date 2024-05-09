#include <csaw/CSaw.hpp>
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
    if (const auto p = std::dynamic_pointer_cast<DereferenceExpression>(ptr))
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

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::Gen(const BinaryExpression& expression)
{
    auto left = Gen(expression.Left);
    auto right = Gen(expression.Right);
    auto op = expression.Operator;

    if (const auto& ref = GetFunction(op, nullptr, {left.GetRawBaseType(), right.GetRawBaseType()}); ref.Function)
    {
        const auto value = m_Builder->CreateCall(ref.Function->getFunctionType(), ref.Function, {left.Load().GetValue(), right.Load().GetValue()});
        return ValueRef::Constant(this, value, ref.Result);
    }

    if (const auto& ref = GetFunction(op, left.Load().GetRawBaseType(), {right.GetRawBaseType()}); ref.Function)
    {
        const auto value = m_Builder->CreateCall(ref.Function->getFunctionType(), ref.Function, {left.GetValue(), right.Load().GetValue()});
        return ValueRef::Constant(this, value, ref.Result);
    }

    if (op == "=")
        return left.Store(right);

    const auto [lhs, rhs] = CastToBestOf(left.Load(), right.Load());

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

    if (value.Invalid())
        CSAW_WIP;

    if (assign)
        return left.Store(value);

    return value;
}

csaw::ValueRef csaw::Builder::Gen(const CallExpression& expression)
{
    std::string name;
    ValueRef callee;
    std::vector<llvm::Value*> args;
    std::vector<TypePtr> arg_raw_types;

    if (const auto identifier = std::dynamic_pointer_cast<IdentifierExpression>(expression.Callee))
    {
        name = identifier->Id;
    }
    else if (const auto member = std::dynamic_pointer_cast<MemberExpression>(expression.Callee))
    {
        name = member->Member;
        callee = Gen(member->Object);
        if (!member->ShouldDeref)
            callee = callee.GetReference();
    }
    else
        CSAW_WIP;

    if (!callee.Invalid())
        args.push_back(callee.GetValue());

    for (const auto& arg : expression.Args)
    {
        const auto value = Gen(arg).Load();
        args.push_back(value.GetValue());
        arg_raw_types.push_back(value.GetRawType());
    }

    const auto& ref = GetFunction(name, callee.Invalid() ? nullptr : callee.GetRawBaseType(), arg_raw_types);
    if (!ref.Function)
        throw std::runtime_error("cannot resolve function '" + name + "'");

    ValueRef output;
    if (ref.IsConstructor)
    {
        output = ValueRef::Allocate(this, nullptr, Type::Get(ref.Name));
        args.insert(args.begin(), output.GetValue());
    }

    const auto value = m_Builder->CreateCall(ref.Function->getFunctionType(), ref.Function, args);

    if (ref.IsConstructor)
        return ValueRef::Constant(this, output.Load().GetValue(), output.GetRawBaseType());
    return ValueRef::Constant(this, value, ref.Result);
}

csaw::ValueRef csaw::Builder::Gen(const CastExpression& expression)
{
    auto value = Gen(expression.Value).Load();
    if (value.GetRawType() == expression.Type)
        return value;

    const auto type = Gen(expression.Type);

    if (value.GetType()->isPointerTy())
    {
        if (type->isPointerTy())
        {
            const auto result = m_Builder->CreatePointerCast(value.GetValue(), type);
            return ValueRef::Constant(this, result, expression.Type);
        }

        if (type->isIntegerTy())
        {
            const auto result = m_Builder->CreatePtrToInt(value.GetValue(), type);
            return ValueRef::Constant(this, result, expression.Type);
        }
    }

    if (value.GetType()->isIntegerTy())
    {
        if (type->isPointerTy())
        {
            const auto result = m_Builder->CreateIntToPtr(value.GetValue(), type);
            return ValueRef::Constant(this, result, expression.Type);
        }

        if (type->isIntegerTy())
        {
            const auto result = m_Builder->CreateIntCast(value.GetValue(), type, true);
            return ValueRef::Constant(this, result, expression.Type);
        }

        if (type->isFloatingPointTy())
        {
            const auto result = m_Builder->CreateSIToFP(value.GetValue(), type);
            return ValueRef::Constant(this, result, expression.Type);
        }
    }

    if (value.GetType()->isFloatingPointTy())
    {
        if (type->isIntegerTy())
        {
            const auto result = m_Builder->CreateFPToSI(value.GetValue(), type);
            return ValueRef::Constant(this, result, expression.Type);
        }

        if (type->isFloatingPointTy())
        {
            const auto result = m_Builder->CreateFPCast(value.GetValue(), type);
            return ValueRef::Constant(this, result, expression.Type);
        }
    }

    CSAW_WIP;
}

csaw::ValueRef csaw::Builder::Gen(const CharExpression& expression)
{
    const auto value = m_Builder->getInt8(expression.Value);
    return ValueRef::Constant(this, value, Type::Get("int8"));
}

csaw::ValueRef csaw::Builder::Gen(const DereferenceExpression& expression)
{
    const auto value = Gen(expression.Value).Load();
    if (!value.GetRawType()->IsPointer())
        throw std::runtime_error("cannot dereference non-pointer");
    const auto deref = m_Builder->CreateLoad(value.GetBaseType(), value.GetValue());
    return ValueRef::Constant(this, deref, value.GetRawBaseType());
}

csaw::ValueRef csaw::Builder::Gen(const FloatExpression& expression)
{
    const auto value = llvm::ConstantFP::get(m_Builder->getDoubleTy(), expression.Value);
    return ValueRef::Constant(this, value, Type::Get("flt64"));
}

csaw::ValueRef csaw::Builder::Gen(const IdentifierExpression& expression)
{
    if (const auto& value = m_Values[expression.Id]; !value.Invalid())
        return value;
    return m_GlobalValues[expression.Id];
}

csaw::ValueRef csaw::Builder::Gen(const IndexExpression& expression)
{
    const auto array = Gen(expression.Array).Load();
    const auto index = Gen(expression.Index).Load();

    const auto value = m_Builder->CreateGEP(array.GetBaseType(), array.GetValue(), {index.GetValue()});
    return ValueRef::Pointer(this, value, array.GetRawBaseType());
}

csaw::ValueRef csaw::Builder::Gen(const IntExpression& expression)
{
    const auto value = m_Builder->getInt32(expression.Value);
    return ValueRef::Constant(this, value, Type::Get("int32"));
}

csaw::ValueRef csaw::Builder::Gen(const MemberExpression& expression)
{
    auto object = Gen(expression.Object);
    if (expression.ShouldDeref)
        object = object.Load();

    auto [index, type] = ElementInStruct(object.GetRawBaseType(), expression.Member);

    const auto value = m_Builder->CreateStructGEP(object.GetBaseType(), object.GetValue(), index);
    return ValueRef::Pointer(this, value, type);
}

csaw::ValueRef csaw::Builder::Gen(const ReferenceExpression& expression)
{
    const auto value = Gen(expression.Value);
    return value.GetReference();
}

csaw::ValueRef csaw::Builder::Gen(const SelectExpression& expression)
{
    const auto condition = Gen(expression.Condition);

    const auto true_block = llvm::BasicBlock::Create(*m_Context, "true", m_Builder->GetInsertBlock()->getParent());
    const auto false_block = llvm::BasicBlock::Create(*m_Context, "false", m_Builder->GetInsertBlock()->getParent());
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end", m_Builder->GetInsertBlock()->getParent());

    m_Builder->CreateCondBr(condition.Load().GetValue(), true_block, false_block);

    m_Builder->SetInsertPoint(true_block);
    const auto true_value = Gen(expression.True).Load();
    m_Builder->CreateBr(end_block);

    m_Builder->SetInsertPoint(false_block);
    const auto false_value = Gen(expression.False).Load();
    m_Builder->CreateBr(end_block);

    m_Builder->SetInsertPoint(end_block);

    const auto phi = m_Builder->CreatePHI(true_value.GetType(), 2);
    phi->addIncoming(true_value.GetValue(), true_block);
    phi->addIncoming(false_value.GetValue(), false_block);

    return ValueRef::Constant(this, phi, true_value.GetRawType());
}

csaw::ValueRef csaw::Builder::Gen(const StringExpression& expression)
{
    const auto value = m_Builder->CreateGlobalString(expression.Value);
    return ValueRef::Constant(this, value, PointerType::Get(Type::Get("int8")));
}

csaw::ValueRef csaw::Builder::Gen(const UnaryExpression& expression)
{
    const auto value = Gen(expression.Value);
    const auto op = expression.Operator;

    ValueRef result;
    if (op == "-") result = GenNeg(value);
    if (op == "!") result = GenNot(value);
    if (op == "~") result = GenInv(value);
    if (op == "++") result = GenInc(value, expression.OpRight);
    if (op == "--") result = GenDec(value, expression.OpRight);

    if (result.Invalid())
        CSAW_WIP;

    return result;
}

csaw::ValueRef csaw::Builder::Gen(const VarArgExpression& expression)
{
    CSAW_WIP;
}
