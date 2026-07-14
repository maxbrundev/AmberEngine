#include "Amberpch.h"

#include "AmberTools/Utils/Random.h"

std::default_random_engine AmberTools::Utils::Random::GENERATOR;

int AmberTools::Utils::Random::Generate(int p_min, int p_max)
{
	std::uniform_int_distribution<int> distribution(p_min, p_max);
	return distribution(GENERATOR);
}

float AmberTools::Utils::Random::Generate(float p_min, float p_max)
{
	std::uniform_real_distribution<float> distribution(p_min, p_max);
	return distribution(GENERATOR);
}

bool AmberTools::Utils::Random::CheckPercentage(float p_percentage)
{
	std::uniform_real_distribution<float> distribution(0.0f, 100.0f);
	return distribution(GENERATOR) <= p_percentage;
}
