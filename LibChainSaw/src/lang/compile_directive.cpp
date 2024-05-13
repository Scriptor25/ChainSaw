#include <filesystem>
#include <fstream>
#include <csaw/CSaw.hpp>
#include <csaw/lang/Parser.hpp>

void csaw::Parser::ParseCompileDirective()
{
    const auto line = m_Line;

    const auto directive = Expect(TK_COMPILE_DIRECTIVE).Value;
    if (directive == "inc")
    {
        const auto filename = Expect(TK_STRING).Value;

        std::filesystem::path filepath = filename;
        std::ifstream stream(filepath);
        for (size_t i = 0; i < m_IncludePaths.size() && !stream.is_open(); ++i)
        {
            filepath = m_IncludePaths[i];
            filepath /= filename;
            stream.open(filepath);
        }

        if (!stream.is_open())
            CSAW_MESSAGE_(true, m_Filename, line, "failed to open include file '" + filename + "', please check your include paths");

        Parse(filepath.string(), stream, m_Callback, m_IncludePaths);
        stream.close();
        return;
    }

    CSAW_MESSAGE_(true, m_Filename, line, "unhandled compile directive: " + directive);
}
