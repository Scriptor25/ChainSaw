#include <iostream>
#include <csaw/Error.hpp>
#include <csaw/Parser.hpp>

void csaw::Tellv(const SourceLoc& loc, const char* format, va_list args)
{
    std::cout << loc.ToString() << ": ";
    vprintf(format, args);
    std::cout << std::endl;
}

void csaw::Tell(const SourceLoc& loc, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    Tellv(loc, format, args);
    va_end(args);
}

void csaw::ThrowErrorv(const SourceLoc& loc, const bool is_severe, const char* format, va_list args)
{
    Tellv(loc, format, args);

    CSawError = 1;
    if (is_severe)
        throw std::runtime_error("Severe Error");
}

void csaw::ThrowError(const SourceLoc& loc, const bool is_severe, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    ThrowErrorv(loc, is_severe, format, args);
    va_end(args);
}

bool csaw::Assert(const bool assertion, const SourceLoc& loc, const bool is_severe, const char* format, ...)
{
    if (assertion)
        return false;

    va_list args;
    va_start(args, format);
    ThrowErrorv(loc, is_severe, format, args);
    va_end(args);

    return true;
}
