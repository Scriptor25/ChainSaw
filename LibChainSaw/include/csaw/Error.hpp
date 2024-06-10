#pragma once

#include <cstdarg>

namespace csaw
{
    int GetError();
    void SetError(int error);

    struct SourceLoc;
    void Tellv(const SourceLoc& loc, const char* format, va_list args);
    void Tell(const SourceLoc& loc, const char* format, ...);
    void ThrowErrorv(const SourceLoc& loc, bool is_severe, const char* format, va_list args);
    void ThrowError(const SourceLoc& loc, bool is_severe, const char* format, ...);
    bool Assert(bool assertion, const SourceLoc& loc, bool is_severe, const char* format, ...);
}

#ifdef __GNUG__
#define TellStmt(_Statement, _Format, ...) csaw::Tell((_Statement).Loc, _Format __VA_OPT__(,) __VA_ARGS__)
#define ThrowErrorStmt(_Statement, _IsSevere, _Format, ...) csaw::ThrowError((_Statement).Loc, _IsSevere, _Format __VA_OPT__(,) __VA_ARGS__)
#define AssertStmt(_Assertion, _Statement, _IsSevere, _Format, ...) csaw::Assert(!!(_Assertion), (_Statement).Loc, _IsSevere, _Format __VA_OPT__(,) __VA_ARGS__)
#else
#define TellStmt(_Statement, _Format, ...) csaw::Tell((_Statement).Loc, _Format, __VA_ARGS__)
#define ThrowErrorStmt(_Statement, _IsSevere, _Format, ...) csaw::ThrowError((_Statement).Loc, _IsSevere, _Format, __VA_ARGS__)
#define AssertStmt(_Assertion, _Statement, _IsSevere, _Format, ...) csaw::Assert(!!(_Assertion), (_Statement).Loc, _IsSevere, _Format, __VA_ARGS__)
#endif
