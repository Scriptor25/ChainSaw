#include <csaw/CSaw.hpp>

csaw::ChainSawMessage::ChainSawMessage(const std::string& source_file, const size_t source_line, const std::string& file, const size_t line, const bool can_recover, const std::string& message)
    : SourceFile(source_file), SourceLine(source_line), Filename(file), Line(line), CanRecover(can_recover), Message(message)
{
}

char const* csaw::ChainSawMessage::what() const
{
    return Message.c_str();
}
