#pragma once

#include <random>

namespace ff7r
{
	// [0.0f, 1.0f) 
	/*__forceinline */float random();

	// [0, max]
	/*__forceinline */int randi(int max);
	/*__forceinline */int randi(int min, int max);

	// [0.0f, max)
	/*__forceinline */float randf(float max);
	/*__forceinline */float randf(float min, float max);
}