#include <cstdarg>
#include <format>
#include <iostream>
#include <csaw/Error.hpp>
#include <csaw/Parser.hpp>

void csaw::ThrowError(const SourceLoc& loc, const bool is_severe, const char* format, ...)
{
    std::cout << loc.ToString() << ": ";

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    std::cout << std::endl;

    CSawError = 1;

    if (is_severe)
        throw std::runtime_error("Severe Error");
}
