#include <lang/Assert.hpp>

#include <iostream>

void csaw::lang::_CSawAssert(const char *expression, const char *message, const char *file, size_t line)
{
    std::cerr << "At line " << line << " in '" << file << "' in '" << expression << "': " << message << std::endl;
#ifdef DEBUG
    throw;
#endif
}
