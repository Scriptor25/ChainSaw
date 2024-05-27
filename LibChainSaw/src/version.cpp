#include <csaw/ChainSaw.hpp>
#include <config/version.h>

void csaw::GetVersion(int& major, int& minor, int& patch)
{
    major = CHAINSAW_VERSION_MAJOR;
    minor = CHAINSAW_VERSION_MINOR;
    patch = CHAINSAW_VERSION_PATCH;
}
