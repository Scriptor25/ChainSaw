#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace csaw
{
    struct Type;
    typedef std::shared_ptr<Type> TypePtr;

    struct FunType;
    typedef std::shared_ptr<FunType> FunTypePtr;

    struct ThingType;
    typedef std::shared_ptr<ThingType> ThingTypePtr;

    std::ostream &operator<<(std::ostream &out, TypePtr ptr);

    struct Type
    {
        static TypePtr Get(const std::string &name);

        virtual ~Type() = default;

        explicit Type(const std::string &name);

        std::string Name;
    };

    struct FunType : Type
    {
        static FunTypePtr Get(const TypePtr &callee,
                              const std::vector<TypePtr> &args,
                              bool vararg,
                              const TypePtr &result);

        FunType(const std::string &name,
                const TypePtr &callee,
                const std::vector<TypePtr> &args,
                bool vararg,
                const TypePtr &result);

        TypePtr Callee;
        std::vector<TypePtr> Args;
        bool VarArg;
        TypePtr Result;
    };

    struct ThingType : Type
    {
        static ThingTypePtr Get(const std::string &name, const std::string &group);

        explicit ThingType(const std::string &name);

        bool Implemented() const;

        std::map<std::string, TypePtr> Elements;
    };
}
