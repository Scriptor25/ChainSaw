#include <csaw/lang/Parser.hpp>

csaw::TypePtr csaw::Parser::ParseType()
{
    if (NextIfAt("%"))
    {
        const auto result = ParseType();

        std::vector<TypePtr> args;
        bool is_vararg = false;

        Expect("(");
        while (!At(")") && !AtEOF())
        {
            if (NextIfAt("?"))
            {
                is_vararg = true;
                break;
            }
            args.push_back(ParseType());
            if (!At(")"))
                Expect(",");
        }
        Expect(")");

        return ParseType(FunctionType::Get(result, args, is_vararg));
    }

    const auto name = Expect(TK_IDENTIFIER).Value;
    return ParseType(Type::Get(name));
}

csaw::TypePtr csaw::Parser::ParseType(const TypePtr& base)
{
    if (NextIfAt("*"))
        return ParseType(PointerType::Get(base));
    if (NextIfAt("["))
    {
        const auto size = Expect(TK_INT_BIN | TK_INT_OCT | TK_INT_DEC | TK_INT_HEX).IntValue();
        Expect("]");
        return ParseType(ArrayType::Get(base, size));
    }

    return base;
}
