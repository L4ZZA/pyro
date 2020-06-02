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

        random(int32_t custom_seed);
        bool init(int32_t custom_seed);

        void seed(int32_t seed);
        uint32_t seed();
        // Returns a random floating point value between 0 and 1.
        float get_float();
        // Returns a random uint32 value between a given range [min,max]
        // default values are int32_t min values and int32_t max value,
        // or between the specified range.
        // Cast the result to uint if you want all positive numbers
        int32_t get_int(int32_t min = 0, int32_t max = s_uint_max) const;

    private:
        uint32_t m_seed;
        bool m_initialized = false;
    };

    inline random::random(int32_t custom_seed)
        : m_seed(custom_seed)
    {
    }

    inline bool random::init(int32_t custom_seed)
    {
        if(m_initialized)
            return false;

        seed(custom_seed);
        m_initialized = true;
        return true;
    }

    inline void random::seed(int32_t seed)
    {
        m_seed = seed;
    }

    inline uint32_t random::seed()
    {
        return m_seed;
    }

    inline float random::get_float()
    {
        std::mt19937 m_random_engine(m_seed);
        std::uniform_int_distribution<int32_t> distribution;
        return static_cast<float>(distribution(m_random_engine)) / static_cast<float>(s_uint_max);
    }

    inline int32_t random::get_int(int32_t min /*= s_uint_min*/, int32_t max /*= s_uint_max*/) const
    {
        std::mt19937 m_random_engine(m_seed);
        std::uniform_int_distribution<int32_t> distribution(min, max);
        return distribution(m_random_engine);
    }

}