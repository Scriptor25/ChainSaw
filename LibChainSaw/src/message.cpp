#include <iostream>
#include <csaw/Message.hpp>

csaw::ChainSawMessage::ChainSawMessage(const std::string& source_file, const size_t source_line, const std::string& file, const size_t line, const std::string& message)
    : SourceFile(source_file), SourceLine(source_line), Filename(file), Line(line), Message(message)
{
}

char const* csaw::ChainSawMessage::what() const noexcept
{
    return Message.c_str();
}

void csaw::MakeMessage(const std::string& source_file, const size_t source_line, const std::string& file, const size_t line, const bool can_recover, const std::string& message)
{
    if (!can_recover)
        throw ChainSawMessage(source_file, source_line, file, line, message);

    std::cout << file << '(' << line << "): " << message << std::endl;
}
