#pragma once
#include <array>
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
	// - added noise_array getter
	// - made most methods const
	// - added methods to retrieve arrays of noise

    class perlin_noise
    {
	public:
		// Generate a new permutation vector based on the value of seed
		perlin_noise(uint32_t seed);
		// Get a noise value, for 2D images z can have any value
		float noise(float x, float y, float z) const;

		// Returns a vector representing a 1d array of noise based on the input values.
		// @param size   one dimensional size of the array. (total_size = size x size)
		// @param scale  value by which the noise can be changed to look like is scaled up and down.
		//               [must be int type]
		// @param morph  [float value] value to modify the noise output
		// @param move_x [float value] displacement on the x azis
		std::vector<float> noise_1d_array(
			const int size, int scale, float morph,
			float move_x = 0.f) const;

		// Returns a vector representing a 2d array of noise based on the input values.
		// @param size   one dimensional size of the array. (total_size = size x size)
		// @param scale  value by which the noise can be changed to look like is scaled up and down.
		//               [must be int type]
		// @param morph  [float value] value to modify the noise output
		// @param move_x [float value] displacement on the x azis
		// @param move_y [float value] displacement on the y azis
		std::vector<float> noise_2d_array(
			const int size, int scale, float morph,
			float move_x = 0.f, float move_y = 0.f) const;

		void change_seed(uint32_t seed);

	private:
		// rendom engine
		std::mt19937 m_engine; // TODO: test with std::mt19937 instead
		// The permutation vector, consists of a vector of unique numbers from 0 to 255
		// with random order. This values are then duplicated to expand the table to 512 values.
		std::vector<int32_t> m_permutation;
    };

}
