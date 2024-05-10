#pragma once

#include <memory>
#include <string>
#include <vector>

namespace csaw
{
    struct Type;
    struct PointerType;
    struct ArrayType;
    struct StructType;

    typedef std::shared_ptr<Type> TypePtr;
    typedef std::shared_ptr<PointerType> PointerTypePtr;
    typedef std::shared_ptr<ArrayType> ArrayTypePtr;
    typedef std::shared_ptr<StructType> StructTypePtr;

    std::ostream& operator<<(std::ostream& out, const TypePtr& ptr);

    struct Type
    {
        static TypePtr Get(const std::string& name);
        static TypePtr GetVoid();
        static TypePtr GetInt1();
        static TypePtr GetInt8();
        static TypePtr GetInt16();
        static TypePtr GetInt32();
        static TypePtr GetInt64();
        static TypePtr GetInt128();
        static TypePtr GetFlt16();
        static TypePtr GetFlt32();
        static TypePtr GetFlt64();
        static TypePtr GetOrCreate(const std::string& name);
        static void Alias(const std::string& name, const TypePtr& origin);

        explicit Type(const std::string& name);
        virtual ~Type() = default;

        [[nodiscard]] virtual bool IsPointer() const;
        [[nodiscard]] virtual bool IsArray() const;
        [[nodiscard]] virtual bool IsStruct() const;

        [[nodiscard]] const PointerType* AsPointer() const;
        [[nodiscard]] const ArrayType* AsArray() const;
        [[nodiscard]] const StructType* AsStruct() const;

        std::string Name;
    };

    struct PointerType : Type
    {
        static PointerTypePtr Get(const TypePtr& base);

        PointerType(const std::string& name, const TypePtr& base);

        [[nodiscard]] bool IsPointer() const override;

        TypePtr Base;
    };

    struct ArrayType : Type
    {
        static ArrayTypePtr Get(const TypePtr& base, size_t size);

        ArrayType(const std::string& name, const TypePtr& base, size_t size);

        [[nodiscard]] bool IsArray() const override;

        TypePtr Base;
        size_t Size;
    };

    struct StructType : Type
    {
        static StructTypePtr Get(const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements);
        static StructTypePtr Get(const std::string& name);

        StructType(const std::string& name, const std::vector<std::pair<std::string, TypePtr>>& elements);

        [[nodiscard]] bool IsStruct() const override;

        std::vector<std::pair<std::string, TypePtr>> Elements;
    };
}
