#include <csaw/codegen/Builder.hpp>
#include <csaw/lang/Expr.hpp>

llvm::Value* csaw::Builder::Gen(const ExpressionPtr& ptr)
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

llvm::Value* csaw::Builder::Gen(const BinaryExpression& expression)
{
    const auto left = Gen(expression.Left);
    const auto right = Gen(expression.Right);

    auto op = expression.Operator;
    if (op == "==") return GenCmpEQ(left, right);
    if (op == "!=") return GenCmpNE(left, right);
    if (op == "<=") return GenCmpLE(left, right);
    if (op == ">=") return GenCmpGE(left, right);
    if (op == "=") return m_Builder->CreateStore(right, left);

    const auto equ_idx = op.find('=');
    const auto assign = equ_idx != std::string::npos;
    if (assign) op = op.substr(0, equ_idx);

    llvm::Value* value = nullptr;

    if (op == "&") value = m_Builder->CreateAnd(left, right);
    if (op == "&&") value = m_Builder->CreateLogicalAnd(left, right);
    if (op == "|") value = m_Builder->CreateOr(left, right);
    if (op == "||") value = m_Builder->CreateLogicalOr(left, right);
    if (op == "^") value = m_Builder->CreateXor(left, right);
    if (op == "<") value = GenCmpLT(left, right);
    if (op == ">") value = GenCmpGT(left, right);
    if (op == "<<") value = m_Builder->CreateShl(left, right);
    if (op == ">>") value = m_Builder->CreateAShr(left, right);
    if (op == "+") value = GenAdd(left, right);
    if (op == "-") value = GenSub(left, right);
    if (op == "*") value = GenMul(left, right);
    if (op == "/") value = GenDiv(left, right);
    if (op == "%") value = GenRem(left, right);

    if (!value)
        throw std::runtime_error("unhandled binary operator");

    if (assign)
        return m_Builder->CreateStore(value, left);
    return value;
}

llvm::Value* csaw::Builder::Gen(const CallExpression& expression)
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
    else throw std::runtime_error("wtf");

    std::vector<TypePtr> argtypes(expression.Args.size());
    for (size_t i = 0; i < argtypes.size(); ++i)
        argtypes[i] = expression.Args[i]->GetType();

    const auto function = GetFunction(name, callee ? callee->GetType() : nullptr, argtypes);

    std::vector<llvm::Value*> args(expression.Args.size() + 1);
    args[0] = callee ? Gen(callee) : llvm::ConstantPointerNull::get(m_Builder->getPtrTy());
    for (size_t i = 1; i < args.size(); ++i)
        args[i] = Gen(expression.Args[i - 1]);

    return m_Builder->CreateCall(function->getFunctionType(), function, args);
}

llvm::Value* csaw::Builder::Gen(const CastExpression& expression)
{
    const auto value = Gen(expression.Value);
    const auto type = Gen(expression.Type);

    if (value->getType()->isIntegerTy())
        return m_Builder->CreateIntCast(value, type, true);
    if (value->getType()->isFloatingPointTy())
        return m_Builder->CreateFPCast(value, type);
    if (value->getType()->isPointerTy())
        return m_Builder->CreatePointerCast(value, type);

    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const CharExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const FloatExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const IdentifierExpression& expression)
{
    return m_Values[expression.Id];
}

llvm::Value* csaw::Builder::Gen(const IndexExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const IntExpression& expression)
{
    return m_Builder->getIntN(64, expression.Value);
}

llvm::Value* csaw::Builder::Gen(const MemberExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const SelectExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const StringExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const UnaryExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const VarArgExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}
