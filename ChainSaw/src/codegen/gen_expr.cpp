#include <csaw/codegen/Builder.hpp>
#include <csaw/lang/Expr.hpp>

llvm::Value* csaw::Builder::Gen(const ExpressionPtr& ptr)
{
    if (const auto p = std::dynamic_pointer_cast<BinaryExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<CallExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<CharExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<IdentifierExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<IndexExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<MemberExpression>(ptr))
        return Gen(*p);
    if (const auto p = std::dynamic_pointer_cast<NumberExpression>(ptr))
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
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const CallExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const CharExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const IdentifierExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const IndexExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const MemberExpression& expression)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::Gen(const NumberExpression& expression)
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
