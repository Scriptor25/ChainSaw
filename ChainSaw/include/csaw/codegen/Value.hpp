#pragma once

#include <csaw/codegen/Def.hpp>
#include <csaw/Type.hpp>

namespace csaw
{
    struct Value
    {
        Value(const std::string &name, const TypePtr &type);

        virtual ~Value();

        std::string Name;
        TypePtr Type;
    };

    struct NumValue : Value
    {
        NumValue(const std::string &name, const TypePtr &type, double x);

        double X;
    };

    struct Arg : Value
    {
        explicit Arg(const std::string &name, const TypePtr &type);
    };
}
