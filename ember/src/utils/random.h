#pragma once
#include <random>

namespace utils
{

    class random
    {
    private:
        inline static constexpr uint32_t s_uint_max = std::numeric_limits<uint32_t>::max();
        inline static constexpr int32_t  s_int_min = std::numeric_limits<int32_t>::min();
        inline static constexpr int32_t  s_int_max = std::numeric_limits<int32_t>::max();

    public:

        random(uint32_t custom_seed);

        void seed(uint32_t seed);
        uint32_t seed() const;
        // Returns a random floating point value between 0 and 1.
        float get_float() const;
        // Returns a random uint32 value between a given range [min,max].
        // Default values are zero for min and int32_t::max for max.
        // Cast the result to uint if you want all positive numbers
        int32_t get_int(int32_t min = 0, int32_t max = s_int_max) const;

    private:
        uint32_t m_seed;
        // This is mutable to allow const method to change its value,
        // as apparently distribution(engine) is not a const method..
        mutable std::mt19937 m_random_engine;
    };

    inline random::random(uint32_t custom_seed)
        : m_seed(custom_seed)
        , m_random_engine(custom_seed)
    {
    }

    inline void random::seed(uint32_t seed)
    {
        m_seed = seed;
        m_random_engine = std::mt19937(seed);
    }

    inline uint32_t random::seed() const
    {
        return m_seed;
    }

    inline float random::get_float() const
    {
        std::uniform_int_distribution<std::mt19937::result_type> distribution;
        auto random_value = distribution(m_random_engine);
        float res = static_cast<float>(random_value) / static_cast<float>(s_uint_max);
        return res;
    }

    inline int32_t random::get_int(int32_t min /*= s_int_min*/, int32_t max /*= s_int_max*/) const
    {
        std::uniform_int_distribution<int32_t> distribution(min, max);
        return distribution(m_random_engine);
    }

}