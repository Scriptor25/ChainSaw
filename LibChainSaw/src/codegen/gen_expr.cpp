#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Signature.hpp>
#include <csaw/codegen/Value.hpp>
#include <csaw/lang/Expr.hpp>

csaw::ValuePtr csaw::Builder::Gen(const ExpressionPtr& ptr)
{
    if (!ptr)
        CSAW_MESSAGE_NONE(true, "expression must not be null");

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

    CSAW_MESSAGE_STMT(true, *ptr, "code generation for expression is not implemented");
}

csaw::RValuePtr csaw::Builder::Gen(const BinaryExpression& expression)
{
    const auto left = Gen(expression.Left);
    const auto right = Gen(expression.Right);
    auto op = expression.Operator;

    const auto lleft = std::dynamic_pointer_cast<LValue>(left);

    if (const auto [signature, function] = FindFunction(op, nullptr, {left->GetType(), right->GetType()}); function)
    {
        const auto value = m_Builder->CreateCall(function->getFunctionType(), function, {left->GetValue(), right->GetValue()});
        return RValue::Create(signature.Result, value);
    }

    if (left->IsLValue())
    {
        if (const auto [signature, function] = FindFunction(op, left->GetType(), {right->GetType()}); function)
        {
            const auto value = m_Builder->CreateCall(function->getFunctionType(), function, {lleft->GetPointer(), right->GetValue()});
            return RValue::Create(signature.Result, value);
        }
    }

    if (op == "=")
    {
        if (!left->IsLValue())
            CSAW_MESSAGE_STMT(true, expression, "cannot assign to rvalue");
        const auto casted = Cast(right, lleft->GetType());
        lleft->StoreValue(casted->GetValue());
        return lleft->GetRValue();
    }

    const auto [lhs, rhs] = CastToBestOf(left->GetRValue(), right->GetRValue());

    RValuePtr value;
    if (op == "==") value = GenCmpEQ(lhs, rhs);
    else if (op == "!=") value = GenCmpNE(lhs, rhs);
    else if (op == "<=") value = GenCmpLE(lhs, rhs);
    else if (op == ">=") value = GenCmpGE(lhs, rhs);
    else
    {
        const auto equ_idx = op.find('=');
        const auto assign = equ_idx != std::string::npos;
        if (assign) op = op.substr(0, equ_idx);

        if (op == "&") value = GenAnd(lhs, rhs);
        else if (op == "&&") value = GenLogicalAnd(lhs, rhs);
        else if (op == "|") value = GenOr(lhs, rhs);
        else if (op == "||") value = GenLogicalOr(lhs, rhs);
        else if (op == "^") value = GenXor(lhs, rhs);
        else if (op == "<") value = GenCmpLT(lhs, rhs);
        else if (op == ">") value = GenCmpGT(lhs, rhs);
        else if (op == "<<") value = GenShl(lhs, rhs);
        else if (op == ">>") value = GenAShr(lhs, rhs);
        else if (op == ">>>") value = GenLShr(lhs, rhs);
        else if (op == "+") value = GenAdd(lhs, rhs);
        else if (op == "-") value = GenSub(lhs, rhs);
        else if (op == "*") value = GenMul(lhs, rhs);
        else if (op == "/") value = GenDiv(lhs, rhs);
        else if (op == "%") value = GenRem(lhs, rhs);

        if (value && assign)
        {
            if (!left->IsLValue())
                CSAW_MESSAGE_STMT(true, expression, "cannot assign to rvalue");
            const auto casted = Cast(value, lleft->GetType());
            lleft->StoreValue(casted->GetValue());
            return lleft->GetRValue();
        }
    }

    if (!value)
        CSAW_MESSAGE_STMT(true, expression, "binary operation " + lhs->GetType()->Name + " " + expression.Operator + " " + rhs->GetType()->Name + " is not implemented");

    return value;
}

csaw::RValuePtr csaw::Builder::Gen(const CallExpression& expression)
{
    std::string name;
    LValuePtr lobject;

    if (const auto identifier = std::dynamic_pointer_cast<IdentifierExpression>(expression.Callee))
    {
        name = identifier->Id;
    }
    else if (const auto member = std::dynamic_pointer_cast<MemberExpression>(expression.Callee))
    {
        name = member->Member;
        const auto object = Gen(member->Object);
        if (object->IsLValue())
        {
            lobject = std::dynamic_pointer_cast<LValue>(object);
            if (member->ShouldDeref)
                lobject = lobject->Dereference();
        }
        else
        {
            if (!member->ShouldDeref)
                CSAW_MESSAGE_STMT(true, expression, "cannot call function on rvalue");
            lobject = object->GetRValue()->Dereference(this);
        }
    }
    else
        CSAW_MESSAGE_STMT(true, expression, "WTF have you done");

    std::vector<ValuePtr> args;
    std::vector<TypePtr> arg_types;
    for (const auto& arg : expression.Args)
    {
        const auto value = Gen(arg);
        args.push_back(value);
        arg_types.push_back(value->GetType());
    }

    const auto [signature, function] = FindFunction(name, lobject ? lobject->GetType() : nullptr, arg_types);
    if (!function)
        CSAW_MESSAGE_STMT(true, expression, "cannot resolve function " + signature.Mangle());

    std::vector<llvm::Value*> vargs;
    LValuePtr lresult;
    if (signature.IsConstructor())
    {
        lresult = LValue::Allocate(this, Type::Get(signature.Name));
        vargs.push_back(lresult->GetPointer());
    }

    if (lobject)
        vargs.push_back(lobject->GetPointer());

    size_t i = 0;
    for (; i < signature.Args.size(); ++i)
        vargs.push_back(Cast(args[i], signature.Args[i])->GetValue());
    for (; i < args.size(); ++i)
        vargs.push_back(args[i]->GetValue());

    const auto result = m_Builder->CreateCall(function->getFunctionType(), function, vargs);

    if (signature.IsConstructor())
        return lresult->GetRValue();

    return RValue::Create(signature.Result, result);
}

csaw::RValuePtr csaw::Builder::Gen(const CastExpression& expression)
{
    const auto value = Gen(expression.Value);
    return Cast(value, expression.Type);
}

csaw::RValuePtr csaw::Builder::Gen(const CharExpression& expression) const
{
    const auto value = m_Builder->getInt8(expression.Value);
    return RValue::Create(Type::GetInt8(), value);
}

csaw::LValuePtr csaw::Builder::Gen(const DereferenceExpression& expression)
{
    const auto value = Gen(expression.Value);
    const auto lvalue = std::dynamic_pointer_cast<LValue>(value);
    if (!value->IsLValue())
        return value->GetRValue()->Dereference(this);
    return lvalue->Dereference();
}

csaw::RValuePtr csaw::Builder::Gen(const FloatExpression& expression) const
{
    const auto value = llvm::ConstantFP::get(m_Builder->getDoubleTy(), expression.Value);
    return RValue::Create(Type::GetFlt64(), value);
}

csaw::LValuePtr csaw::Builder::Gen(const IdentifierExpression& expression)
{
    if (const auto& value = m_Values[expression.Id])
        return value;
    if (const auto& value = m_GlobalValues[expression.Id])
        return value;
    CSAW_MESSAGE_STMT(true, expression, "undefined identifier '" + expression.Id + "'");
}

csaw::LValuePtr csaw::Builder::Gen(const IndexExpression& expression)
{
    const auto array = Gen(expression.Array);
    const auto index = Gen(expression.Index);

    if (array->IsLValue())
    {
        if (const auto [signature, function] = FindFunction("[]", array->GetType(), {index->GetType()}); function)
        {
            if (!signature.Result->IsPointer())
                CSAW_MESSAGE_STMT(true, expression, "function overloading index operator must return pointer type");

            const auto larray = std::dynamic_pointer_cast<LValue>(array);
            const auto value = m_Builder->CreateCall(function->getFunctionType(), function, {larray->GetPointer(), index->GetValue()});
            return LValue::Direct(this, signature.Result->AsPointer()->Base, value);
        }
    }

    if (!array->GetType()->IsPointer())
        CSAW_MESSAGE_STMT(true, expression, "cannot compute offset from non-pointer type");

    const auto type = array->GetType()->AsPointer()->Base;
    const auto pointer = m_Builder->CreateGEP(Gen(type), array->GetValue(), {index->GetValue()});
    return LValue::Direct(this, type, pointer);
}

csaw::RValuePtr csaw::Builder::Gen(const IntExpression& expression) const
{
    const auto value = m_Builder->getInt32(expression.Value);
    return RValue::Create(Type::GetInt32(), value);
}

csaw::LValuePtr csaw::Builder::Gen(const MemberExpression& expression)
{
    const auto object = Gen(expression.Object);
    LValuePtr lobject;
    if (object->IsLValue())
    {
        lobject = std::dynamic_pointer_cast<LValue>(object);
        if (expression.ShouldDeref)
            lobject = lobject->Dereference();
    }
    else
    {
        if (!expression.ShouldDeref)
            CSAW_MESSAGE_STMT(true, expression, "cannot get member from rvalue");
        lobject = object->GetRValue()->Dereference(this);
    }

    if (!lobject->GetType()->IsStruct())
        CSAW_MESSAGE_STMT(true, expression, "cannot get member of non-struct type");

    const auto [eindex, etype] = lobject->GetType()->AsStruct()->GetElement(expression.Member);

    if (eindex < 0 || !etype)
        CSAW_MESSAGE_STMT(true, expression, "type " + lobject->GetType()->Name + " does not have member '" + expression.Member + "'");

    const auto pointer = m_Builder->CreateStructGEP(Gen(lobject->GetType()), lobject->GetPointer(), eindex);
    return LValue::Direct(this, etype, pointer);
}

csaw::RValuePtr csaw::Builder::Gen(const ReferenceExpression& expression)
{
    const auto lvalue = std::dynamic_pointer_cast<LValue>(Gen(expression.Value));
    if (!lvalue)
        CSAW_MESSAGE_STMT(true, expression, "cannot get reference to rvalue");
    return lvalue->GetReference();
}

csaw::ValuePtr csaw::Builder::Gen(const SelectExpression& expression)
{
    const auto condition = Gen(expression.Condition);

    auto true_block = llvm::BasicBlock::Create(*m_Context, "true", m_Builder->GetInsertBlock()->getParent());
    auto false_block = llvm::BasicBlock::Create(*m_Context, "false", m_Builder->GetInsertBlock()->getParent());
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end", m_Builder->GetInsertBlock()->getParent());

    m_Builder->CreateCondBr(condition->GetValue(), true_block, false_block);

    m_Builder->SetInsertPoint(true_block);
    const auto true_value = Gen(expression.True);
    const auto true_rvalue = true_value->GetRValue();
    true_block = m_Builder->GetInsertBlock();
    m_Builder->CreateBr(end_block);

    m_Builder->SetInsertPoint(false_block);
    const auto false_value = Gen(expression.False);
    const auto false_rvalue = false_value->GetRValue();
    false_block = m_Builder->GetInsertBlock();
    m_Builder->CreateBr(end_block);

    m_Builder->SetInsertPoint(end_block);

    if (true_value->GetType() != false_value->GetType())
        CSAW_MESSAGE_STMT(true, expression, "cannot use select with results of differing types");

    if (true_value->IsLValue() && false_value->IsLValue())
    {
        const auto true_lvalue = std::dynamic_pointer_cast<LValue>(true_value);
        const auto false_lvalue = std::dynamic_pointer_cast<LValue>(false_value);

        const auto phi = m_Builder->CreatePHI(Gen(true_value->GetType()), 2);
        phi->addIncoming(true_lvalue->GetPointer(), true_block);
        phi->addIncoming(false_lvalue->GetPointer(), false_block);

        return LValue::Direct(this, true_value->GetType(), phi);
    }

    const auto phi = m_Builder->CreatePHI(Gen(true_value->GetType()), 2);
    phi->addIncoming(true_rvalue->GetValue(), true_block);
    phi->addIncoming(false_rvalue->GetValue(), false_block);

    return RValue::Create(true_value->GetType(), phi);
}

csaw::RValuePtr csaw::Builder::Gen(const StringExpression& expression) const
{
    const auto value = m_Builder->CreateGlobalStringPtr(expression.Value);
    return RValue::Create(PointerType::Get(Type::GetInt8()), value);
}

csaw::RValuePtr csaw::Builder::Gen(const UnaryExpression& expression)
{
    const auto value = Gen(expression.Value);
    const auto op = expression.Operator;

    const auto lvalue = std::dynamic_pointer_cast<LValue>(value);
    if (lvalue)
    {
        if (expression.OpRight) // get and op (@(++):vec3(t: int1): vec3 ...)
        {
            if (const auto [signature, function] = FindFunction(op, value->GetType(), {Type::GetInt1()}); function)
            {
                const auto result = m_Builder->CreateCall(function->getFunctionType(), function, {lvalue->GetPointer(), m_Builder->getInt1(true)});
                return RValue::Create(signature.Result, result);
            }
        }
        else // op and get (@(++):vec3:vec3 ...)
        {
            if (const auto [signature, function] = FindFunction(op, value->GetType(), {}); function)
            {
                const auto result = m_Builder->CreateCall(function->getFunctionType(), function, {lvalue->GetPointer()});
                return RValue::Create(signature.Result, result);
            }
        }
    }

    const auto rvalue = value->GetRValue();

    RValuePtr result;
    if (op == "-") result = GenNeg(rvalue);
    else if (op == "!") result = GenNot(rvalue);
    else if (op == "~") result = GenInv(rvalue);
    else if (op == "++")
    {
        result = GenInc(rvalue);
        if (result)
        {
            if (!value->IsLValue())
                CSAW_MESSAGE_STMT(true, expression, "cannot assign to rvalue");
            const auto casted = Cast(result, lvalue->GetType());
            lvalue->StoreValue(casted->GetValue());
            if (expression.OpRight) result = rvalue;
        }
    }
    else if (op == "--")
    {
        result = GenDec(rvalue);
        if (result)
        {
            if (!value->IsLValue())
                CSAW_MESSAGE_STMT(true, expression, "cannot assign to rvalue");
            const auto casted = Cast(result, lvalue->GetType());
            lvalue->StoreValue(casted->GetValue());
            if (expression.OpRight) result = rvalue;
        }
    }

    if (!result)
    {
        const auto ty = value->GetType()->Name;
        const std::string left = expression.OpRight ? ty : op;
        const std::string right = expression.OpRight ? op : ty;
        CSAW_MESSAGE_STMT(true, expression, "unary operation " + left + right + " not implemented");
    }

    return result;
}
