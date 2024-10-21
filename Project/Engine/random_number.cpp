#include "pch.h"
#include "random_number.h"

namespace ff7r
{
	std::mt19937 gen(std::random_device{}());
	std::uniform_real_distribution<float> dis(0.0, 1.0);

	float random()
	{
		return dis(gen);
	}

	std::random_device i_rd;
	std::mt19937 i_gen(i_rd());

	int randi(int max)
	{
		std::uniform_int_distribution<int> distr(0, max);
		return distr(i_gen);
	}

	int randi(int min, int max)
	{
		std::uniform_int_distribution<int> distr(min, max);
		return distr(i_gen);
	}

	std::random_device f_rd;
	std::mt19937 f_gen(f_rd());

	float randf(float max)
	{
		std::uniform_real_distribution<float> distr(0, max);
		return distr(f_gen);
	}
	float randf(float min, float max)
	{
		std::uniform_real_distribution<float> distr(min, max);
		return distr(f_gen);
	}
}