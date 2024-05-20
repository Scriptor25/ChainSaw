#include <cstdarg>
#include <format>
#include <iostream>
#include <csaw/Error.hpp>

void csaw::ThrowError(const std::string& file, const size_t line, const bool is_error, const char* format, ...)
{
    if (!file.empty())
    {
        std::cout << file;
        if (line == 0) std::cout << ": ";
    }
    if (line != 0) std::cout << '(' << line << "): ";

    std::cout << (is_error ? "Error" : "Warning") << ": ";

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    std::cout << std::endl;

    if (is_error)
        throw std::runtime_error("Error");
}
