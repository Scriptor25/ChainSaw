#include <filesystem>
#include <fstream>
#include <csaw/Error.hpp>
#include <csaw/Parser.hpp>

void csaw::Parser::ParseCompileDirective()
{
    const auto loc = m_Loc;

    const auto directive = Expect(TK_COMPILE_DIRECTIVE).Value;
    if (directive == "inc")
    {
        const auto filename = Expect(TK_STRING).Value;

        std::filesystem::path filepath = filename;
        std::ifstream stream(filepath);
        if (!stream.is_open())
        {
            filepath = m_Data.Filename;
            filepath = filepath.parent_path() / filename;
            stream.open(filepath);
        }
        for (size_t i = 0; i < m_Data.IncludePaths.size() && !stream.is_open(); ++i)
        {
            filepath = m_Data.IncludePaths[i];
            filepath /= filename;
            stream.open(filepath);
        }

        if (!stream.is_open())
            return ThrowError(loc, true, "Failed to open include file '%s'", filename.c_str());

        Parse({filepath.string(), stream, m_Data.Callback, m_Data.IncludePaths, m_Data.Processed});
        stream.close();
        return;
    }

    ThrowError(loc, false, "Undefined compile directive '%s'", directive.c_str());
}
