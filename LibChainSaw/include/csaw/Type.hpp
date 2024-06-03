#pragma once

#include <string>
#include <vector>
#include <csaw/Def.hpp>

namespace csaw
{
    struct Type
            : std::enable_shared_from_this<Type>
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

        static void Alias(const std::string& name, const TypePtr& origin);

        explicit Type(const std::string& name, int info);
        virtual ~Type() = default;

        [[nodiscard]] bool IsVoid() const;
        [[nodiscard]] bool IsInt() const;
        [[nodiscard]] bool IsFlt() const;
        [[nodiscard]] bool IsPointer() const;
        [[nodiscard]] bool IsArray() const;
        [[nodiscard]] bool IsStruct() const;
        [[nodiscard]] bool IsFunction() const;
        [[nodiscard]] bool IsTemplate() const;
        bool IsFunctionPointer();

        PointerTypePtr AsPointer();
        ArrayTypePtr AsArray();
        StructTypePtr AsStruct();
        FunctionTypePtr AsFunction();
        TemplateTypePtr AsTemplate();

        bool ParentOf(const TypePtr& type);

        std::string Name;
        int Info;
    };

    struct PointerType : Type
    {
        static PointerTypePtr Get(const TypePtr& base);

        PointerType(const std::string& name, const TypePtr& base);

        TypePtr Base;
    };

    struct ArrayType : Type
    {
        static ArrayTypePtr Get(const TypePtr& base, size_t size);

        ArrayType(const std::string& name, const TypePtr& base, size_t size);

        TypePtr Base;
        size_t Size;
    };

    struct StructType : Type
    {
        static StructTypePtr Create(const std::string& name, const std::vector<Arg>& elements);
        static StructTypePtr Get(const std::string& name);

        StructType(const std::string& name, const std::vector<Arg>& elements);

        [[nodiscard]] std::pair<int, TypePtr> GetElement(const std::string& name) const;

        std::vector<Arg> Elements;
    };

    struct FunctionType : Type
    {
        static FunctionTypePtr Get(const std::vector<TypePtr>& args, bool is_vararg, const TypePtr& parent, const TypePtr& result);

        FunctionType(const std::string& name, const std::vector<TypePtr>& args, bool is_vararg, const TypePtr& parent, const TypePtr& result);

        std::vector<TypePtr> Args;
        bool IsVararg;
        TypePtr Parent;
        TypePtr Result;
    };

    struct TemplateType : Type
    {
        static TemplateTypePtr Get(const TypePtr& base, const std::vector<TypePtr>& args);

        TemplateType(const std::string& name, const TypePtr& base, const std::vector<TypePtr>& args);

        TypePtr Base;
        std::vector<TypePtr> Args;
    };
}
