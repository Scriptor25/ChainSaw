#pragma once

#include <memory>
#include <string>

namespace csaw
{
    struct Type;
    struct PointerType;

    typedef std::shared_ptr<Type> TypePtr;
    typedef std::shared_ptr<PointerType> PointerTypePtr;

    std::ostream& operator<<(std::ostream& out, const TypePtr& ptr);

    struct Type
    {
        static TypePtr Get(const std::string& name);
        static void Alias(const std::string& name, const TypePtr& origin);

        explicit Type(const std::string& name);
        virtual ~Type() = default;

        std::string Name;
    };

    struct PointerType : Type
    {
        static PointerTypePtr Get(const TypePtr& base);

        PointerType(const std::string& name, const TypePtr& base);

        TypePtr Base;
    };
}
