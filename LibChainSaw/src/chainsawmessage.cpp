#include <csaw/CSaw.hpp>

csaw::ChainSawMessage::ChainSawMessage(const char* file, const size_t line, const bool canRecover, const std::string& message)
    : File(file), Line(line), CanRecover(canRecover), Message(message)
{
}

char const* csaw::ChainSawMessage::what() const
{
    return Message.c_str();
}
