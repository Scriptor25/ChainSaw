#pragma once

#include <memory>
#include <string>
#include <vector>

namespace csaw
{
    struct Type;
    struct PointerType;
    struct StructType;

    typedef std::shared_ptr<Type> TypePtr;
    typedef std::shared_ptr<PointerType> PointerTypePtr;
    typedef std::shared_ptr<StructType> StructTypePtr;

    std::ostream& operator<<(std::ostream& out, const TypePtr& ptr);

    struct Type
    {
        static TypePtr Get(const std::string& name);
        static void Alias(const std::string& name, const TypePtr& origin);

        explicit Type(const std::string& name);
        virtual ~Type() = default;

        virtual bool IsPointer() const;
        virtual bool IsStruct() const;

        std::string Name;
    };

    struct PointerType : Type
    {
        static PointerTypePtr Get(const TypePtr& base);

        PointerType(const std::string& name, const TypePtr& base);

        bool IsPointer() const override;

        TypePtr Base;
    };

    struct StructType : Type
    {
        static StructTypePtr Get(const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements);
        static StructTypePtr Get(const std::string& name);

        StructType(const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements);

        bool IsStruct() const override;

        std::vector<std::pair<std::string, TypePtr>> Elements;
    };
}
