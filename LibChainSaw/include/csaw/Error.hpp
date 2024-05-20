#pragma once

#include <string>

namespace csaw
{
    void ThrowError(const std::string& file, size_t line, bool is_error, const char* format, ...);
}

#define ThrowErrorStmt(_Statement, _IsError, _Format, ...) csaw::ThrowError((_Statement).Filename, (_Statement).Line, _IsError, _Format, __VA_ARGS__)
