#include "perlin_noise.h"

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
// - moved interpolation methods to math utils 

#include <cmath>
#include <algorithm>
#include <numeric>
#include "utils/math.h"

// Generate a new permutation vector based on the value of seed
utils::perlin_noise::perlin_noise(uint32_t seed)
	: m_engine(seed)
{
	//m_engine.seed(seed);

	// Initialize the permutation vector with the reference values
	m_permutation = 
	{
		151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
		8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
		35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
		134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
		55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
		18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
		250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
		189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
		43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
		97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
		107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 
	};
	// Duplicate the permutation vector
	m_permutation.insert(m_permutation.end(), m_permutation.begin(), m_permutation.end());
}

float utils::perlin_noise::noise(float x, float y, float z) const
{
	// Find the unit cube that contains the point
	int32_t X = (int32_t)floor(x) & 255;
	int32_t Y = (int32_t)floor(y) & 255;
	int32_t Z = (int32_t)floor(z) & 255;

	// Find relative x, y, z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x, y, z
	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	// Hash coordinates of the 8 cube corners
	int32_t A = m_permutation[X] + Y;
	int32_t AA = m_permutation[A] + Z;
	int32_t AB = m_permutation[A + 1] + Z;
	int32_t B = m_permutation[X + 1] + Y;
	int32_t BA = m_permutation[B] + Z;
	int32_t BB = m_permutation[B + 1] + Z;

	// Add blended results from 8 corners of cube
	float res = lerp(w, 
					lerp(v, 
						lerp(u, 
							grad(m_permutation[AA], x, y, z), 
							grad(m_permutation[BA], x - 1, y, z)), 
						lerp(u, 
							grad(m_permutation[AB], x, y - 1, z), 
							grad(m_permutation[BB], x - 1, y - 1, z))
						), 
					lerp(v, 
						lerp(u, 
							grad(m_permutation[AA + 1], x, y, z - 1), 
							grad(m_permutation[BA + 1], x - 1, y, z - 1)),
						lerp(u, 
							grad(m_permutation[AB + 1], x, y - 1, z - 1), 
							grad(m_permutation[BB + 1], x - 1, y - 1, z - 1)))
				 );
	return (res + 1.0) / 2.0;
}

std::vector<float> utils::perlin_noise::noise_1d_array(
	const int size,
	int scale,
	float morph,
	float move_x /*= 0.f*/) const
{
	std::vector<float> result;
	result.resize(size);

	// other noise 
	// Visit every pixel of the image and assign a color generated with Perlin noise 
	int pos = 0;
	for(unsigned int x = 0; x < size; ++x)
	{  // y 
		float dx = static_cast<float>(x) / (static_cast<float>(size));

		// Typical Perlin noise 
		float n = noise(scale * dx - move_x, 0.f, morph);

		//// Wood like structure 
		//n = 20 * m_other_noise.noise(x, y, m_something);
		//n = n - floor(n);

		result[pos] = n;
		pos++;
	}
	return std::move(result);
}
std::vector<float> utils::perlin_noise::noise_2d_array(
	const int size,
	int scale,
	float morph,
	float move_x /*= 0.f*/,
	float move_y /*= 0.f*/) const
{
	std::vector<float> result;
	int new_size = size * size;
	result.resize(new_size);

	// other noise 
	// Visit every pixel of the image and assign a color generated with Perlin noise 
	int pos = 0;
	for(unsigned int y = 0; y < size; ++y)
	{     // x 
		for(unsigned int x = 0; x < size; ++x)
		{  // y 
			float dx = static_cast<float>(x) / (static_cast<float>(size));
			float dy = static_cast<float>(y) / (static_cast<float>(size));

			// Typical Perlin noise 
			float n = noise(scale * dx - move_x, scale * dy + move_y, morph);

			//// Wood like structure 
			//n = 20 * m_other_noise.noise(x, y, m_something);
			//n = n - floor(n);

			result[pos] = n;
			pos++;
		}
	}
	return std::move(result);
}

void utils::perlin_noise::change_seed(uint32_t seed)
{
	// Initialize a random engine with seed
	m_engine.seed(seed);
	
	m_permutation.resize(256);

	// Fill permutation_vec with values from 0 to 255
	std::iota(m_permutation.begin(), m_permutation.end(), 0);

	// Suffle  using the above random engine
	std::shuffle(m_permutation.begin(), m_permutation.end(), m_engine);

	// Duplicate the permutation vector
	m_permutation.insert(m_permutation.end(), m_permutation.begin(), m_permutation.end());
}
