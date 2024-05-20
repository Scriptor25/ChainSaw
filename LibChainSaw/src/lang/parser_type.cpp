#include <csaw/lang/Parser.hpp>

#include "csaw/Error.hpp"

csaw::TypePtr csaw::Parser::ParseType()
{
    if (NextIfAt("%"))
    {
        TypePtr result;

        if (!At("("))
            result = ParseType();

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

            TypePtr arg_type;
            if (At(TK_IDENTIFIER))
            {
                std::string arg_name = Get().Value;
                if (NextIfAt(":"))
                    arg_type = ParseType();
                else
                {
                    const auto type = Type::Get(arg_name);
                    if (!type)
                        ThrowError(m_Data.Filename, m_Line, true, "Failed to get type %s", arg_name.c_str());
                    arg_type = ParseType(type);
                }
            }
            else arg_type = ParseType();

            args.push_back(arg_type);
            if (!At(")"))
                Expect(",");
        }
        Expect(")");

        return ParseType(FunctionType::Get(result, args, is_vararg));
    }

    const auto name = Expect(TK_IDENTIFIER).Value;
    const auto type = Type::Get(name);
    if (!type)
        ThrowError(m_Data.Filename, m_Line, true, "Failed to get type %s", name.c_str());
    return ParseType(type);
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
