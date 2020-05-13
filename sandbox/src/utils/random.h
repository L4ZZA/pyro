#pragma once

#include <random>

namespace utils{

	class random
	{
	private:
		inline static constexpr uint32_t s_uint_max = std::numeric_limits<uint32_t>::max();
		inline static constexpr int32_t  s_int_min  = std::numeric_limits<int32_t>::min();
		inline static constexpr int32_t  s_int_max  = std::numeric_limits<int32_t>::max();

	public:
		static bool init();
		static bool init(uint32_t custom_seed);

		static void seed(uint32_t seed);

		// Returns a random floating point value between 0 and 1.
		static float get_float();
		// Returns a random uint32 value between a given range [min,max]
		// default values are int32_t min values and int32_t max value,
		// or between the specified range.
		// Cast the result to uint if you want all positive numbers
		static int32_t get_int(int32_t min = 0, int32_t max = s_uint_max);

	private:
		inline static bool s_initialized = false;
		inline static std::mt19937 s_random_engine;
		inline static std::uniform_int_distribution<std::mt19937::result_type> s_distribution;
	};


	inline bool random::init()
	{
		if (s_initialized)
			return false;

		s_random_engine.seed(std::random_device()());
		s_initialized = true;
		return true;
	}

	inline bool random::init(uint32_t custom_seed)
	{
		if (s_initialized)
			return false;

		seed(custom_seed);
		s_initialized = true;
		return true;
	}

	inline void random::seed(uint32_t seed)
	{
		s_random_engine.seed(seed);
	}

	inline float random::get_float()
	{
		return static_cast<float>(s_distribution(s_random_engine)) / static_cast<float>(s_uint_max);
	}

	inline int32_t random::get_int(int32_t min /*= s_uint_min*/, int32_t max /*= s_uint_max*/)
	{
		std::uniform_int_distribution<std::mt19937::result_type> distribution(min,max);
		return distribution(s_random_engine);
	}

}