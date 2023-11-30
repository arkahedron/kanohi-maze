#include "Randomizer.h"
#include <random>
#include <windows.h>
#include <string>
#include <iostream>


using namespace std;
using u32 = uint_least32_t;
using engine = std::mt19937;

Randomizer::Randomizer()
{

}
Randomizer::~Randomizer()
{

}

int Randomizer::Generate(int min, int max)
{
	int randResult;
	random_device os_seed;
	const u32 seed = os_seed();
	engine generator(seed);
	uniform_int_distribution< u32 > distribute(min, max);
	randResult = distribute(generator);

	return randResult;
}

int Randomizer::Modulate()
{
	int randResult;
	random_device os_seed;
	const u32 seed = os_seed();
	engine generator(seed);
	uniform_int_distribution< u32 > distribute(0,2);
	randResult = distribute(generator);
	randResult -= 1;

	return randResult;
}
