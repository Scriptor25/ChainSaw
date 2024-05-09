#include <sstream>
#include <stdexcept>
#include <csaw/CSaw.hpp>

void csaw::csaw_wip(const char* file, const size_t line)
{
    std::ostringstream message;
    message << "In " << file << ':' << line << ": not yet implemented";
    throw std::runtime_error(message.str());
}
