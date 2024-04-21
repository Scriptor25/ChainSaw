#pragma once

#include <lang/Def.hpp>

#include <string>

namespace csaw::lang
{
    struct Type
    {
        static TypePtr Get(const std::string &name);

        virtual ~Type() = default;

        explicit Type(const std::string &name);

        std::string Name;
    };
}
