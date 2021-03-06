#pragma once
#pragma warning( disable : 26451 )
#include <array>
#include <cmath>
#include "utils/random.h"

namespace utils
{

    /* Function to linearly interpolate between a and b
     * The weight parameter should be in the range [0.0, 1.0]
     */
    inline float lerp(float a, float b, float weight)
    {
        return (1.0f - weight) * a + weight * b;
    }
    
    inline std::vector<float> generate_seed(int size, 
                              uint32_t seed,
                              float average = 0.0f)
   {
        utils::random rand(seed);
        std::vector<float> seed_array;
        seed_array.resize(size);
        
        for(int i = 0; i < size; i++)
        {
            seed_array[i] = rand.get_float();
        }
        if(average > 0.f)
        {
            //this will average the noise around the given value
            //(i.e. average = 0.5f, the noise array will start and finish at 0.5)
            seed_array[0] = average;
        }

        return std::move(seed_array);
    }

    inline void perlin_noise_1d(int output_size, int octaves, float bias, 
                                uint32_t seed, float* output_noise,
                                float average = 0.0f)
    {
        std::vector<float> seed_array = generate_seed(output_size, seed, average);

        for (int x = 0; x < output_size; x++)
        {
            float noise = 0.f;
            float scale = 1.f;
            float scale_accumulator = 0.f;

            for (int oct = 0; oct < octaves; oct++)
            {
                // shifting by octave, meaning deviding by 2 the number of 
                // time specified by the octave value
                const int pitch = output_size >> oct; 
                // intger division
                const int sample1 = (x / pitch) * pitch;
                const int sample1_bis = (x / pitch) * pitch;
                const int sample2 = (sample1 + pitch) % output_size; // modulos to wrap around

                const float blend = static_cast<float>(x - sample1) / static_cast<float>(pitch);
                const float sample = lerp(seed_array[sample1], seed_array[sample2], blend);

                scale_accumulator += scale;
                noise += sample * scale;
                scale = scale / bias;
            }

            output_noise[x] = noise / scale_accumulator;
        }
    }

    inline void perlin_noise_2d(int output_size, int octaves, float bias, 
                                uint32_t seed, float* output_noise,
                                float average = 0.0f)
    {
        std::vector<float> seed_array = generate_seed(output_size * output_size, seed, average);

        for (int y = 0; y < output_size; y++)
            for (int x = 0; x < output_size; x++)
            {
                float noise = 0.f;
                float scale = 1.f;
                // We accumulate the scale values to then use it as denominator
                // to normalize the value between 0 and 1; 
                float scale_accumulator = 0.f;

                for (int oct = 0; oct < octaves; oct++)
                {
                    // shifting by octave, meaning deviding by 2 the number of 
                    // time specified by the octave value
                    const int pitch = output_size >> oct;

                    // integer division 
                    const int sample_x1 = (x / pitch) * pitch;
                    const int sample_y1 = (y / pitch) * pitch;
                    const int sample_x2 = (sample_x1 + pitch) % output_size; // modulos to wrap around
                    const int sample_y2 = (sample_y1 + pitch) % output_size; // modulos to wrap around

                    const float blend_x = 
                        static_cast<float>(x - sample_x1) / static_cast<float>(pitch);
                    const float blend_y = 
                        static_cast<float>(y - sample_y1) / static_cast<float>(pitch);

                    // both samples use the same 
                    const float sample_t = 
                        lerp(
                            seed_array[static_cast<int>(sample_y1 * output_size + sample_x1)], 
                            seed_array[static_cast<int>(sample_y1 * output_size + sample_x2)], 
                            blend_x);
                    const float sample_b = 
                        lerp(
                            seed_array[sample_y2 * output_size + sample_x1], 
                            seed_array[sample_y2 * output_size + sample_x2], 
                            blend_x);

                    scale_accumulator += scale;
                    noise += (blend_y * (sample_b - sample_t) + sample_t) * scale;
                    scale = scale / bias;
                }

                // Scale to seed range
                output_noise[y * output_size + x] = noise / scale_accumulator;
            }
    }
}