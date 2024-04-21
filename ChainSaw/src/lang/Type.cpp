#include <lang/Parser.hpp>
#include <lang/Type.hpp>

#include <map>

static std::map<std::string, csaw::lang::TypePtr> Types;

csaw::lang::TypePtr csaw::lang::Parser::ParseType()
{
    auto name = Expect(TK_IDENTIFIER).Value;
    return Type::Get(name);
}

csaw::lang::TypePtr csaw::lang::Type::Get(const std::string &name)
{
    auto &type = Types[name];
    if (type) return type;
    return type = std::make_shared<Type>(name);
}

csaw::lang::Type::Type(const std::string &name)
        : Name(name)
{
}
