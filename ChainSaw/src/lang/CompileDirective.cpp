#include <csaw/lang/Assert.hpp>
#include <csaw/lang/Parser.hpp>

#include <filesystem>
#include <fstream>

void csaw::Parser::ParseCompileDirective()
{
    auto directive = Expect(TK_COMP_DIR).Value;
    if (directive == "inc")
    {
        const auto filename = Expect(TK_STRING).Value;
        std::ifstream stream(filename);
        for (size_t i = 0; i < m_IncludePaths.size() && !stream.is_open(); ++i)
        {
            std::filesystem::path path(m_IncludePaths[i]);
            path = path / filename;
            stream.open(path);
        }
        CHAINSAW_ASSERT(stream.is_open(), "Failed to include file");

        Parse(stream, m_Callback, m_IncludePaths);
        stream.close();
        return;
    }
}
