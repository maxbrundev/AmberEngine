#pragma once

#include <random>

namespace AmberTools::Utils
{
	class Random
	{
	public:
		Random() = delete;

		static int Generate(int p_min, int p_max);
		static float Generate(float p_min, float p_max);
		static bool CheckPercentage(float p_percentage);

	private:
		static std::default_random_engine GENERATOR;
	};
}
