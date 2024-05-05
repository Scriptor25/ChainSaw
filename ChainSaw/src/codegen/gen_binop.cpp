#include <csaw/codegen/Builder.hpp>

llvm::Value* csaw::Builder::GenCmpEQ(llvm::Value* left, llvm::Value* right)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::GenCmpNE(llvm::Value* left, llvm::Value* right)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::GenCmpLE(llvm::Value* left, llvm::Value* right)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::GenCmpGE(llvm::Value* left, llvm::Value* right)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::GenCmpLT(llvm::Value* left, llvm::Value* right)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::GenCmpGT(llvm::Value* left, llvm::Value* right)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::GenAdd(llvm::Value* left, llvm::Value* right) const
{
    if (left->getType()->isFloatingPointTy() && right->getType()->isFloatingPointTy())
        return m_Builder->CreateFAdd(left, right);

    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::GenSub(llvm::Value* left, llvm::Value* right) const
{
    if (left->getType()->isFloatingPointTy() && right->getType()->isFloatingPointTy())
        return m_Builder->CreateFSub(left, right);

    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::GenMul(llvm::Value* left, llvm::Value* right) const
{
    if (left->getType()->isFloatingPointTy() && right->getType()->isFloatingPointTy())
        return m_Builder->CreateFMul(left, right);

    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::GenDiv(llvm::Value* left, llvm::Value* right)
{
    throw std::runtime_error("not yet implemented");
}

llvm::Value* csaw::Builder::GenRem(llvm::Value* left, llvm::Value* right)
{
    throw std::runtime_error("not yet implemented");
}
