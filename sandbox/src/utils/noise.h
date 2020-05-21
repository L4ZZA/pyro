#pragma once
#include <array>

enum class e_noise_type
{
    one_d,
    two_d
};

namespace utils
{

    /* Function to linearly interpolate between a and b
     * The weight parameter should be in the range [0.0, 1.0]
     */
    inline float lerp(float a, float b, float weight)
    {
        return (1.0f - weight) * a + weight * b;
    }

    inline void perlin_noise_1d(int output_size, float octaves, float bias, float* seed, float* output_noise)
    {
        for (int x = 0; x < output_size; x++)
        {
            float noise = 0.f;
            float scale = 1.f;
            float scale_accumulator = 0.f;

            for (int oct = 0; oct < octaves; oct++)
            {
                // shifting by octave, meaning deviding by 2 the number of time specified by the octave value
                const int pitch = output_size >> oct; 
                // intger division
                const int sample1 = (x / pitch) * pitch;
                const int sample1_bis = (x / pitch) * pitch;
                const int sample2 = (sample1 + pitch) % output_size; // modulos to wrap around

                const float blend = static_cast<float>(x - sample1) / static_cast<float>(pitch);
                const float sample = lerp(seed[sample1], seed[sample2], blend);

                scale_accumulator += scale;
                noise += sample * scale;
                scale = scale / bias;
            }

            output_noise[x] = noise / scale_accumulator;
        }
    }

    inline void perlin_noise_2d(int size, float octaves, float bias, float* seed, float* output_noise)
    {
        for (int y = 0; y < size; y++)
            for (int x = 0; x < size; x++)
            {
                float noise = 0.f;
                float scale = 1.f;
                // We accumulate the scale values to then use it as denominator to normalize the value between 0 and 1; 
                float scale_accumulator = 0.f;

                for (int oct = 0; oct < octaves; oct++)
                {
                    // shifting by octave, meaning deviding by 2 the number of time specified by the octave value
                    const int pitch = size >> oct;

                    // integer division 
                    const int sample_x1 = (x / pitch) * pitch;
                    const int sample_y1 = (y / pitch) * pitch;
                    const int sample_x2 = (sample_x1 + pitch) % size; // modulos to wrap around
                    const int sample_y2 = (sample_y1 + pitch) % size; // modulos to wrap around

                    const float blend_x = static_cast<float>(x - sample_x1) / static_cast<float>(pitch);
                    const float blend_y = static_cast<float>(y - sample_y1) / static_cast<float>(pitch);

                    // both samples use the same 
                    const float sample_t = lerp(seed[sample_y1 * size + sample_x1], seed[sample_y1 * size + sample_x2], blend_x);
                    const float sample_b = lerp(seed[sample_y2 * size + sample_x1], seed[sample_y2 * size + sample_x2], blend_x);

                    scale_accumulator += scale;
                    noise += (blend_y * (sample_b - sample_t) + sample_t) * scale;
                    scale = scale / bias;
                }

                // Scale to seed range
                output_noise[y * size + x] = noise / scale_accumulator;
            }
    }
}