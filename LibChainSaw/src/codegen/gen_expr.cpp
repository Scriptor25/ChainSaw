#include <csaw/Message.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/Signature.hpp>
#include <csaw/codegen/Value.hpp>
#include <csaw/lang/Expr.hpp>

csaw::ValuePtr csaw::Builder::Gen(const ExpressionPtr& ptr)
{
    if (!ptr)
        return nullptr;

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
    if (const auto p = std::dynamic_pointer_cast<SizeOfExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<StringExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<UnaryExpression>(ptr))
        return Gen(*p);

    CSAW_MESSAGE_STMT(true, *ptr, "Code generation for expression is not implemented");
    return nullptr;
}

csaw::RValuePtr csaw::Builder::Gen(const BinaryExpression& expression)
{
    const auto left = Gen(expression.Left);
    const auto right = Gen(expression.Right);
    auto op = expression.Operator;

    if (!left || !right)
        return nullptr;

    if (const auto [signature, function] = FindFunction(op, nullptr, {left->GetType(), right->GetType()}); function)
    {
        const auto rleft = Cast(left, signature.Args[0]);
        const auto rright = Cast(right, signature.Args[1]);
        const auto value = m_Builder->CreateCall(function->getFunctionType(), function, {rleft->GetValue(), rright->GetValue()});
        return RValue::Create(signature.Result, value);
    }

    const auto lleft = std::dynamic_pointer_cast<LValue>(left);

    if (left->IsLValue())
    {
        if (const auto [signature, function] = FindFunction(op, left->GetType(), {right->GetType()}); function)
        {
            const auto rright = Cast(right, signature.Args[0]);
            const auto value = m_Builder->CreateCall(function->getFunctionType(), function, {lleft->GetPointer(), rright->GetValue()});
            return RValue::Create(signature.Result, value);
        }
    }

    if (op == "=")
    {
        if (!left->IsLValue())
        {
            CSAW_MESSAGE_STMT(true, expression, "Cannot assign to rvalue");
            return nullptr;
        }
        const auto rleft = Cast(right, lleft->GetType());
        if (!rleft)
        {
            CSAW_MESSAGE_STMT(true, expression, "Failed to cast");
            return nullptr;
        }
        lleft->StoreValue(rleft->GetValue());
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
            {
                CSAW_MESSAGE_STMT(true, expression, "Cannot assign to rvalue");
                return nullptr;
            }
            const auto rleft = Cast(value, lleft->GetType());
            if (!rleft)
            {
                CSAW_MESSAGE_STMT(true, expression, "Failed to cast");
                return nullptr;
            }
            lleft->StoreValue(rleft->GetValue());
            return lleft->GetRValue();
        }
    }

    if (!value)
    {
        CSAW_MESSAGE_STMT(true, expression, "Binary operation " + lhs->GetType()->Name + " " + expression.Operator + " " + rhs->GetType()->Name + " is not implemented");
        return nullptr;
    }

    return value;
}

csaw::RValuePtr csaw::Builder::Gen(const CallExpression& expression)
{
    std::string name;
    LValuePtr lobject;

    if (const auto identifier_expression = std::dynamic_pointer_cast<IdentifierExpression>(expression.Callee))
    {
        name = identifier_expression->Id;
    }
    else if (const auto member_expression = std::dynamic_pointer_cast<MemberExpression>(expression.Callee))
    {
        name = member_expression->Member;
        const auto object = Gen(member_expression->Object);
        if (!object) return nullptr;

        if (object->IsLValue())
        {
            lobject = std::dynamic_pointer_cast<LValue>(object);
            if (member_expression->ShouldDeref)
            {
                lobject = lobject->Dereference();
                if (!lobject)
                {
                    CSAW_MESSAGE_STMT(true, expression, "Failed to dereference lvalue");
                    return nullptr;
                }
            }
        }
        else
        {
            if (member_expression->ShouldDeref)
            {
                lobject = object->GetRValue()->Dereference(this);
                if (!lobject)
                {
                    CSAW_MESSAGE_STMT(true, expression, "Failed to dereference rvalue");
                    return nullptr;
                }
            }
            else
                lobject = LValue::AllocateAndStore(this, object->GetType(), object->GetValue());
        }
    }
    else
    {
        CSAW_MESSAGE_STMT(true, expression, "WTF have you done");
        return nullptr;
    }

    std::vector<ValuePtr> args;
    std::vector<TypePtr> arg_types;
    for (const auto& arg : expression.Args)
    {
        const auto value = Gen(arg);
        if (!value) return nullptr;
        args.push_back(value);
        arg_types.push_back(value->GetType());
    }

    const auto [signature, function] = FindFunction(name, lobject ? lobject->GetType() : nullptr, arg_types);
    if (!function)
    {
        CSAW_MESSAGE_STMT(true, expression, "Failed to find function with signature '" + signature.Mangle() + "'");
        return nullptr;
    }

    std::vector<llvm::Value*> vargs;
    LValuePtr lresult;
    if (signature.IsConstructor())
    {
        const auto type = Type::Get(signature.Name);
        if (!type)
        {
            CSAW_MESSAGE_STMT(true, expression, "Cannot call constructor with unresolved type");
            return nullptr;
        }
        lresult = LValue::Allocate(this, type);
        vargs.push_back(lresult->GetPointer());
    }

    if (lobject)
        vargs.push_back(lobject->GetPointer());

    size_t i = 0;
    for (; i < signature.Args.size(); ++i)
    {
        const auto casted = Cast(args[i], signature.Args[i]);
        if (!casted)
        {
            CSAW_MESSAGE_STMT(true, expression, "Failed to cast arg");
            return nullptr;
        }
        vargs.push_back(casted->GetValue());
    }
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
    if (!value)
        return nullptr;
    const auto casted = Cast(value, expression.Type);
    if (!casted)
        CSAW_MESSAGE_STMT(true, expression, "Failed to cast value");
    return casted;
}

csaw::RValuePtr csaw::Builder::Gen(const CharExpression& expression) const
{
    const auto value = m_Builder->getInt8(expression.Value);
    return RValue::Create(Type::GetInt8(), value);
}

csaw::LValuePtr csaw::Builder::Gen(const DereferenceExpression& expression)
{
    const auto value = Gen(expression.Value);
    if (!value) return nullptr;
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
    if (const auto value = m_Module->getFunction(expression.Id))
    {
        const auto type = FromLLVM(value->getFunctionType());
        if (!type)
        {
            CSAW_MESSAGE_STMT(true, expression, "Failed to get type for function reference");
            return nullptr;
        }
        return LValue::Direct(this, type, value);
    }

    CSAW_MESSAGE_STMT(true, expression, "Undefined identifier '" + expression.Id + "'");
    return nullptr;
}

csaw::LValuePtr csaw::Builder::Gen(const IndexExpression& expression)
{
    const auto array = Gen(expression.Array);
    const auto index = Gen(expression.Index);

    if (!array || !index)
        return nullptr;

    if (const auto [signature, function] = FindFunction("[]", array->GetType(), {index->GetType()}); function)
    {
        if (!signature.Result->IsPointer())
        {
            CSAW_MESSAGE_STMT(true, expression, "Function overloading index operator must return pointer type");
            return nullptr;
        }

        const auto rindex = Cast(index, signature.Args[0]);
        if (!rindex)
        {
            CSAW_MESSAGE_STMT(true, expression, "Failed to cast index");
            return nullptr;
        }

        LValuePtr larray;
        if (array->IsLValue())
            larray = std::dynamic_pointer_cast<LValue>(array);
        else
            larray = LValue::AllocateAndStore(this, array->GetType(), array->GetValue());

        const auto value = m_Builder->CreateCall(function->getFunctionType(), function, {larray->GetPointer(), rindex->GetValue()});
        return LValue::Direct(this, signature.Result->AsPointer()->Base, value);
    }

    if (!array->GetType()->IsPointer())
    {
        CSAW_MESSAGE_STMT(true, expression, "Cannot compute offset from non-pointer value");
        return nullptr;
    }

    const auto type = array->GetType()->AsPointer()->Base;
    const auto vty = Gen(type);
    if (!vty)
    {
        CSAW_MESSAGE_STMT(true, expression, "Failed to generate type for array element");
        return nullptr;
    }
    const auto pointer = m_Builder->CreateGEP(vty, array->GetValue(), {index->GetValue()});
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
    if (!object) return nullptr;

    LValuePtr lobject;
    if (object->IsLValue())
    {
        lobject = std::dynamic_pointer_cast<LValue>(object);
        if (expression.ShouldDeref)
        {
            lobject = lobject->Dereference();
            if (!lobject)
            {
                CSAW_MESSAGE_STMT(true, expression, "Failed to dereference object lvalue");
                return nullptr;
            }
        }
    }
    else
    {
        if (expression.ShouldDeref)
        {
            lobject = object->GetRValue()->Dereference(this);
            if (!lobject)
            {
                CSAW_MESSAGE_STMT(true, expression, "Failed to dereference object rvalue");
                return nullptr;
            }
        }
        else lobject = LValue::AllocateAndStore(this, object->GetType(), object->GetValue());
    }

    if (!lobject->GetType()->IsStruct())
    {
        CSAW_MESSAGE_STMT(true, expression, "cannot get member of non-struct type");
        return nullptr;
    }

    const auto [eindex, etype] = lobject->GetType()->AsStruct()->GetElement(expression.Member);

    if (eindex < 0 || !etype)
    {
        CSAW_MESSAGE_STMT(true, expression, "type " + lobject->GetType()->Name + " does not have member '" + expression.Member + "'");
        return nullptr;
    }

    const auto type = Gen(lobject->GetType());
    if (!type)
    {
        CSAW_MESSAGE_STMT(true, expression, "Failed to generate object type");
        return nullptr;
    }

    const auto pointer = m_Builder->CreateStructGEP(type, lobject->GetPointer(), eindex);
    return LValue::Direct(this, etype, pointer);
}

csaw::RValuePtr csaw::Builder::Gen(const ReferenceExpression& expression)
{
    const auto value = Gen(expression.Value);
    if (!value) return nullptr;
    const auto lvalue = std::dynamic_pointer_cast<LValue>(value);
    if (!lvalue)
    {
        CSAW_MESSAGE_STMT(true, expression, "cannot get reference to rvalue");
        return nullptr;
    }
    return lvalue->GetReference();
}

csaw::ValuePtr csaw::Builder::Gen(const SelectExpression& expression)
{
    const auto condition = Gen(expression.Condition);
    if (!condition) return nullptr;

    const auto parent = m_Builder->GetInsertBlock()->getParent();
    const auto bkp_block = m_Builder->GetInsertBlock();

    auto true_block = llvm::BasicBlock::Create(*m_Context, "true", parent);
    auto false_block = llvm::BasicBlock::Create(*m_Context, "false");
    const auto end_block = llvm::BasicBlock::Create(*m_Context, "end");

    m_Builder->CreateCondBr(condition->GetBoolValue(this), true_block, false_block);

    m_Builder->SetInsertPoint(true_block);
    const auto true_value = Gen(expression.True);
    if (!true_value)
    {
        true_block->eraseFromParent();
        m_Builder->SetInsertPoint(bkp_block);
        return nullptr;
    }
    auto true_rvalue = true_value->GetRValue();
    true_block = m_Builder->GetInsertBlock();

    false_block->insertInto(parent);
    m_Builder->SetInsertPoint(false_block);
    const auto false_value = Gen(expression.False);
    if (!false_value)
    {
        true_block->eraseFromParent();
        false_block->eraseFromParent();
        m_Builder->SetInsertPoint(bkp_block);
        return nullptr;
    }
    auto false_rvalue = false_value->GetRValue();
    false_block = m_Builder->GetInsertBlock();

    if (false_rvalue->GetType()->ParentOf(true_rvalue->GetType()))
    {
        m_Builder->SetInsertPoint(true_block);
        true_rvalue = Cast(true_rvalue, false_rvalue->GetType());
        if (!true_rvalue)
        {
            true_block->eraseFromParent();
            false_block->eraseFromParent();
            m_Builder->SetInsertPoint(bkp_block);
            CSAW_MESSAGE_STMT(true, expression, "Failed to cast true value");
            return nullptr;
        }
    }
    else
    {
        m_Builder->SetInsertPoint(false_block);
        false_rvalue = Cast(false_rvalue, true_rvalue->GetType());
        if (!false_rvalue)
        {
            true_block->eraseFromParent();
            false_block->eraseFromParent();
            m_Builder->SetInsertPoint(bkp_block);
            CSAW_MESSAGE_STMT(true, expression, "Failed to cast false value");
            return nullptr;
        }
    }

    m_Builder->SetInsertPoint(true_block);
    m_Builder->CreateBr(end_block);

    m_Builder->SetInsertPoint(false_block);
    m_Builder->CreateBr(end_block);

    end_block->insertInto(parent);
    m_Builder->SetInsertPoint(end_block);

    if (true_value->IsLValue() && false_value->IsLValue())
    {
        if (true_rvalue->GetType() != false_rvalue->GetType())
        {
            true_block->eraseFromParent();
            false_block->eraseFromParent();
            end_block->eraseFromParent();
            m_Builder->SetInsertPoint(bkp_block);
            CSAW_MESSAGE_STMT(true, expression, "Cannot use select with lvalues of differing types");
            return nullptr;
        }

        const auto true_lvalue = std::dynamic_pointer_cast<LValue>(true_value);
        const auto false_lvalue = std::dynamic_pointer_cast<LValue>(false_value);

        const auto phi = m_Builder->CreatePHI(m_Builder->getPtrTy(), 2);
        phi->addIncoming(true_lvalue->GetPointer(), true_block);
        phi->addIncoming(false_lvalue->GetPointer(), false_block);
        return LValue::Direct(this, true_lvalue->GetType(), phi);
    }

    const auto phi = m_Builder->CreatePHI(Gen(true_rvalue->GetType()), 2);
    phi->addIncoming(true_rvalue->GetValue(), true_block);
    phi->addIncoming(false_rvalue->GetValue(), false_block);
    return RValue::Create(true_rvalue->GetType(), phi);
}

csaw::RValuePtr csaw::Builder::Gen(const SizeOfExpression& expression) const
{
    const auto type = Gen(expression.Type);
    if (!type)
    {
        CSAW_MESSAGE_STMT(true, expression, "Failed to generate type");
        return nullptr;
    }
    const auto size = m_Module->getDataLayout().getTypeAllocSize(type);
    return RValue::Create(Type::GetInt64(), m_Builder->getInt64(size));
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

    if (!value)
        return nullptr;

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
            {
                CSAW_MESSAGE_STMT(true, expression, "Cannot assign to rvalue");
                return nullptr;
            }
            const auto casted = Cast(result, lvalue->GetType());
            if (!casted)
            {
                CSAW_MESSAGE_STMT(true, expression, "Failed to cast result");
                return nullptr;
            }
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
            {
                CSAW_MESSAGE_STMT(true, expression, "Cannot assign to rvalue");
                return nullptr;
            }
            const auto casted = Cast(result, lvalue->GetType());
            if (!casted)
            {
                CSAW_MESSAGE_STMT(true, expression, "Failed to cast result");
                return nullptr;
            }
            lvalue->StoreValue(casted->GetValue());
            if (expression.OpRight) result = rvalue;
        }
    }

    if (!result)
    {
        const auto ty = value->GetType()->Name;
        const std::string left = expression.OpRight ? ty : op;
        const std::string right = expression.OpRight ? op : ty;

        CSAW_MESSAGE_STMT(true, expression, "Unary operation " + left + right + " not implemented");
        return nullptr;
    }

    return result;
}
