#pragma once

namespace csaw
{
    struct SourceLoc;
    void ThrowError(const SourceLoc& loc, bool is_error, const char* format, ...);
}

#define ThrowErrorStmt(_Statement, _IsError, _Format, ...) csaw::ThrowError((_Statement).Loc, _IsError, _Format, __VA_ARGS__)
