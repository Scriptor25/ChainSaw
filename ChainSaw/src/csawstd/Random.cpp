#include <csawstd/Random.h>
#include <runtime/Const.h>

#include <chrono>
#include <random>

using namespace csaw::runtime;

std::mt19937_64 RNG;
std::uniform_real_distribution<> UDIST;

void csawstd::InitRandom()
{
	RNG.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

	Environment::CreateFunction(false, "random", "num", {}, false, "", csawstd::random);
}

ValuePtr csawstd::random(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args)
{
	double r = UDIST(RNG);
	return ConstNum::Ptr(r);
}
