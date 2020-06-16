#pragma once
#include <stdint.h>

namespace utils
{
    inline float fade(float t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    /* Function to linearly interpolate between a and b
     * The weight parameter should be in the range [0.0, 1.0]
     */
    inline float lerp(float weight, float a, float b)
    {
        return a + weight * (b - a);
    }

    inline float grad(int32_t hash, float x, float y, float z)
    {
        int32_t h = hash & 15;
        // Convert lower 4 bits of hash into 12 gradient directions
        float u = h < 8 ? x : y,
            v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
}