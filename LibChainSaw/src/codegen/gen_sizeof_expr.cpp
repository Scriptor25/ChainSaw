#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::ValuePtr csaw::Builder::Gen(const SizeOfExpression& expression)
{
    const auto type = Gen(expression.Type);
    if (AssertStmt(type, expression, false, "Failed to generate type %s: %s", expression.Type->Name, type.Msg().c_str()))
        return nullptr;

    const auto size = GetModule().getDataLayout().getTypeAllocSize(type.Get());
    return RValue::Create(this, Type::GetInt64(), GetBuilder().getInt64(size));
}
