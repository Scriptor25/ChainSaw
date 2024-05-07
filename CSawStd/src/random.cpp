#include <cstdlib>
#include <csawstd/CSawStd.hpp>

extern "C" double random()
{
    return (double)rand() / RAND_MAX;
}
