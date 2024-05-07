#include <cstdlib>
#include <cstring>
#include <csawstd/CSawStd.hpp>

extern "C" int csaw_strcmp(const char* str1, const char* str2)
{
    return strcmp(str1, str2);
}

extern "C" int csaw_atoi(const char* str)
{
    return atoi(str);
}
