#include <csawstd/CSawStd.h>
#include <csawstd/Img.h>
#include <csawstd/IO.h>
#include <csawstd/Math.h>
#include <csawstd/Random.h>
#include <csawstd/Types.h>

void csawstd::InitLib()
{
	InitImg();
	InitIO();
	InitMath();
	InitRandom();
	InitTypes();
}
