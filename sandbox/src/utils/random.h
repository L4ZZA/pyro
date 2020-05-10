#pragma once

#include <random>

namespace utils{

class random
{
public:
	static void init()
	{
		s_random_engine.seed(std::random_device()());
	}

	// Returns a random floating point value between zero and uint32::max
	static float get_float()
	{
		return static_cast<float>(s_distribution(s_random_engine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	}

private:
	inline static std::mt19937 s_random_engine;
	inline static std::uniform_int_distribution<std::mt19937::result_type> s_distribution;
};
}