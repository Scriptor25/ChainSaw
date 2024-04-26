#pragma once

#include <cstddef>

namespace csaw
{
    void _CSawAssert(const char *expression, const char *message, const char *file, size_t line);
}

#define CHAINSAW_ASSERT(_Expression, _Message) \
 (void) \
 ((!!(_Expression)) || \
  (csaw::_CSawAssert(#_Expression,_Message,__FILE__,__LINE__),0))
