#include <lang/Parser.hpp>
#include <lang/Type.hpp>

#include <map>

csaw::lang::TypePtr csaw::lang::Parser::ParseType()
{
    auto name = Expect(TK_IDENTIFIER).Value;
    return Type::Get(name);
}

csaw::lang::TypePtr csaw::lang::Type::Get(const std::string &name)
{
    static std::map<std::string, TypePtr> types;

    auto &type = types[name];
    if (type) return type;
    return type = std::make_shared<Type>(name);
}

csaw::lang::Type::Type(const std::string &name)
        : Name(name)
{
}
