#pragma once

#include <cstdint>
#include <ctime>

class Random 
{
public: 
    Random();
    Random(uint64_t seed);

    void SetSeed(uint64_t seed);
    void SetRandomSeed();
    void Skip(int32_t n); //Se salta los n siguientes numeros generados
    uint64_t GetSeed();
    int32_t NextInt();
    int32_t NextInt(int32_t max);
    int32_t NextInt(int32_t min, int32_t max);
    int32_t NextIntSign();
    int32_t RangeInt(int32_t max);
    int32_t RangeInt(int32_t min, int32_t max);
    float NextFloat();
    float NextFloat(float max);
    float NextFloat(float min, float max);
    float NextFloatSign();
	uint64_t Next();

    /*
    int32_t RangeInt(int32_t max);
    int32_t RangeInt(int32_t min, int32_t max);
    float RangeFloat(float max);
    float RangeFloat(float min, float max);
    */

    static Random GlobalInstance;
protected:

    uint64_t seed { 0 };
	uint64_t current { 0 };
};
