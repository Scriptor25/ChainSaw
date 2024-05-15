#include <csaw/CSaw.hpp>
#include <csaw/Type.hpp>
#include <csaw/codegen/Builder.hpp>

std::pair<int, csaw::TypePtr> csaw::Builder::ElementInStruct(const csaw::TypePtr& type, const std::string& element)
{
    if (!type->IsStruct())
        CSAW_MESSAGE_NONE(true, "type " + type->Name + " is not a struct");

    int i = 0;
    for (const auto& [ename, etype] : type->AsStruct()->Elements)
    {
        if (ename == element)
            return {i, etype};
        ++i;
    }

    CSAW_MESSAGE_NONE(true, type->Name + " does not have a member '" + element + "'");
}
