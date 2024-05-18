#include <filesystem>
#include <fstream>
#include <csaw/Message.hpp>
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
            return CSAW_MESSAGE_(true, m_Data.Filename, line, "Failed to open include file '" + filename + "', please check your include paths");

        Parse({filepath.string(), stream, m_Data.Callback, m_Data.IncludePaths, m_Data.Processed});
        stream.close();
        return;
    }

    CSAW_MESSAGE_(true, m_Data.Filename, line, "Unhandled compile directive '" + directive + "'");
}
