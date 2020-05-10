#pragma once
#include <vector>

namespace utils
{
	//	You could use this program under the terms of GPL v3, for more details see :
	//  http://www.gnu.org/copyleft/gpl.html
	//  Copyright 2012 Sol from https://solarianprogrammer.com/
    class perlin_noise
    {
	public:
		// Initialize with the reference values for the permutation vector
		perlin_noise();
		// Generate a new permutation vector based on the value of seed
		perlin_noise(uint32_t seed);
		// Get a noise value, for 2D images z can have any value
		double noise(double x, double y, double z);

	private:
		double fade(double t);
		double lerp(double t, double a, double b);
		double grad(int32_t hash, double x, double y, double z);

	private:
		// The permutation vector
		std::vector<int32_t> m_permutation;
    };

}
