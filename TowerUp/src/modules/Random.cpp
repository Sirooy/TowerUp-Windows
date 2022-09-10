#include "Random.h"

#include <limits>

Random::Random() : seed(std::time(nullptr)), current(0)
{
    current = seed;
}
    
Random::Random(uint64_t seed) : seed(seed), current(seed) { }

void Random::SetSeed(uint64_t seed)
{
    this->seed = this->current = seed;
}

void Random::SetRandomSeed()
{
    seed = current = std::time(nullptr);
}

void Random::Skip(int32_t n)
{
	for(int32_t i = 0;i < n; ++i)
		Next();
}

uint64_t Random::GetSeed() { return seed; }

float Random::NextFloat()
{
	return static_cast<float>(Next()) / static_cast<float>(std::numeric_limits<uint64_t>::max());
}

float Random::NextFloat(float max)
{
	return Next() / (static_cast<float>(std::numeric_limits<uint64_t>::max()) / (max));
}

float Random::NextFloat(float min, float max)
{
	return min + Next() / (static_cast<float>(std::numeric_limits<uint64_t>::max()) / (max - min));
}

float Random::NextFloatSign()
{
	return (NextInt(2) == 0) ? 1.0f : -1.0f;
}

int32_t Random::NextInt()
{
	return static_cast<int32_t>(Next());
}

int32_t Random::NextInt(int32_t max)
{
	return Next() % (max);
}

int32_t Random::NextInt(int32_t min, int32_t max)
{
	return (Next() % (max - min)) + min;
}

int32_t Random::NextIntSign()
{
	return (NextInt(2) == 0) ? 1 : -1;
}


int32_t Random::RangeInt(int32_t max)
{
    return Next() % (max + 1);
}

int32_t Random::RangeInt(int32_t min, int32_t max)
{
    return (Next() % (max - min + 1)) + min;
}

// https://lemire.me/blog/2019/03/19/the-fastest-conventional-random-number-generator-that-can-pass-big-crush/
uint64_t Random::Next()
{
	current += 0xe120fc15;
	uint64_t tmp;
	tmp = current * 0x4a39b70d;
	uint64_t m1 = (tmp >> 32) ^ tmp;
	tmp = m1 * 0x12fad5c9;
	uint64_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}

Random Random::GlobalInstance {};