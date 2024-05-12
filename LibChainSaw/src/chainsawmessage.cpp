#include <csaw/CSaw.hpp>

csaw::ChainSawMessage::ChainSawMessage(const std::string& file, const size_t line, const std::string& sourceFile, const size_t sourceLine, const bool canRecover, const std::string& message)
    : File(file), Line(line), SourceFile(sourceFile), SourceLine(sourceLine), CanRecover(canRecover), Message(message)
{
}

char const* csaw::ChainSawMessage::what() const
{
    return Message.c_str();
}
