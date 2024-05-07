#include <cstdarg>
#include <cstdio>
#include <csawstd/CSawStd.hpp>

extern "C" int csaw_printf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    const auto written = vprintf(fmt, args);
    va_end(args);
    return written;
}

extern "C" int csaw_scanf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    const auto written = vscanf(fmt, args);
    va_end(args);
    return written;
}
