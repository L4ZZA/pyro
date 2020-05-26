#pragma once
#include <vector>
#include <random>

namespace utils
{
	// You could use this program under the terms of GPL v3, for more details see :
	// http://www.gnu.org/copyleft/gpl.html
	// Copyright 2012 Sol from https://solarianprogrammer.com/
	// https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
	// https://github.com/sol-prog/Perlin_Noise
	// CHANGES:
	// - changed all parameter and return types from double to float, I don't need that precision.
	// - random engine is now a member variable
	// - added ability to change random engine seed

    class perlin_noise
    {
	public:
		// Generate a new permutation vector based on the value of seed
		perlin_noise(uint32_t seed);
		// Get a noise value, for 2D images z can have any value
		float noise(float x, float y, float z);

		void change_seed(uint32_t seed);

	private:
		float fade(float t);
		float lerp(float t, float a, float b);
		float grad(int32_t hash, float x, float y, float z);

	private:
		// rendom engine
		std::default_random_engine m_engine; // TODO: test with std::mt19937 instead
		// The permutation vector, consists of a vector of unique numbers from 0 to 255
		// with random order. This values are then duplicated to expand the table to 512 values.
		std::vector<int32_t> m_permutation;
    };

}
