#include <cstdarg>
#include <format>
#include <iostream>
#include <csaw/Error.hpp>
#include <csaw/Parser.hpp>

void csaw::ThrowError(const SourceLoc& loc, const bool is_error, const char* format, ...)
{
    std::cout << loc.ToString() << ": " << (is_error ? "Error" : "Warning") << ": ";

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    std::cout << std::endl;

    if (is_error)
        throw std::runtime_error("Error");
}
