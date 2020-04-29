#pragma once
#include <array>

namespace helpers
{

    /* Function to linearly interpolate between a and b
     * The weight parameter should be in the range [0.0, 1.0]
     */
    inline float lerp(float a, float b, float weight)
    {
        return (1.0f - weight) * a + weight * b;
    }

    inline void perlin_noise_1d(int output_size, float octaves, float* seed, float* output_noise)
    {
        for (int x = 0; x < output_size; x++)
        {
            float noise = 0.f;
            float scale = 1.f;
            float scale_accumulator = 0.f;

            for (int oct = 0; oct < octaves; oct++)
            {
                // shifting by octave, meaning deviding by 2 the number of time specified by the octave value
                int pitch = output_size >> oct; 
                // intger division
                int sample1 = (x / pitch) * pitch;
                int sample2 = (sample1 + pitch) % pitch; // modulos to wrap around

                float blend = static_cast<float>(x - sample1) / pitch;
                float sample = lerp(seed[sample1], seed[sample2], blend);

                noise += sample * scale;
                scale_accumulator += scale;
                scale = scale * 0.5f;
            }

            output_noise[x] = noise / scale_accumulator;
        }
    }

}