#include <csaw/lang/Parser.hpp>

csaw::TypePtr csaw::Parser::ParseType()
{
    const auto name = Expect(TK_IDENTIFIER).Value;
    return ParseType(Type::Get(name));
}

csaw::TypePtr csaw::Parser::ParseType(const TypePtr& base)
{
    if (NextIfAt("*"))
        return ParseType(PointerType::Get(base));
    if (NextIfAt("["))
    {
        const auto token = Expect(TK_INT_BIN | TK_INT_OCT | TK_INT_DEC | TK_INT_HEX);
        const auto size = token.IntValue();
        Expect("]");
        return ParseType(ArrayType::Get(base, size));
    }

    return base;
}
