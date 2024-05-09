#pragma once

#define CSAW_WIP csaw_wip(__FILE__, __LINE__)

namespace csaw
{
    [[noreturn]] void csaw_wip(const char* file, size_t line);
}
