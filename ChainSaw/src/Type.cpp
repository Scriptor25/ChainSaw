#include <csaw/lang/Parser.hpp>
#include <csaw/Type.hpp>

#include <map>

static std::map<std::string, csaw::TypePtr> Types;

csaw::TypePtr csaw::Parser::ParseType()
{
    const auto name = Expect(TK_IDENTIFIER).Value;
    return Type::Get(name);
}

csaw::TypePtr csaw::Type::Get(const std::string &name)
{
    auto &type = Types[name];
    if (type) return type;
    return type = std::make_shared<Type>(name);
}

csaw::Type::Type(const std::string &name)
        : Name(name)
{
}

csaw::FunTypePtr csaw::FunType::Get(const TypePtr &callee,
                                    const std::vector<TypePtr> &args,
                                    const bool vararg,
                                    const TypePtr &result)
{
    std::string name;
    name += (callee ? callee->Name : "void");
    name += '(';
    for (size_t i = 0; i < args.size(); ++i)
    {
        if (i > 0) name += ',';
        name += args[i]->Name;
    }
    if (vararg) name += ",?";
    name += ')';
    name += ':';
    name += (result ? result->Name : "void");
    auto &type = Types[name];
    if (!type)
        type = std::make_shared<FunType>(name, callee, args, vararg, result);
    return std::dynamic_pointer_cast<FunType>(type);
}

csaw::FunType::FunType(const std::string &name,
                       const TypePtr &callee,
                       const std::vector<TypePtr> &args,
                       const bool vararg,
                       const TypePtr &result)
        : Type(name),
          Callee(callee),
          Args(args),
          VarArg(vararg),
          Result(result)
{
}

csaw::ThingTypePtr csaw::ThingType::Get(const std::string &name, const std::string &group)
{
    std::string tyName = '[' + name + ':' + group + ']';
    auto &type = Types[tyName];
    if (!type)
        type = std::make_shared<ThingType>(tyName);
    return std::dynamic_pointer_cast<ThingType>(type);
}

csaw::ThingType::ThingType(const std::string &name)
        : Type(name)
{
}

bool csaw::ThingType::Implemented() const
{
    return !Elements.empty();
}
