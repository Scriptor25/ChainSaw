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

    return base;
}
