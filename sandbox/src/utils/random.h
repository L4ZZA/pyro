#pragma once
#include <random>

namespace utils
{

    class random
    {
    private:
        inline static constexpr uint64_t s_uint_max = std::numeric_limits<uint64_t>::max();
        inline static constexpr int64_t  s_int_min = std::numeric_limits<int64_t>::min();
        inline static constexpr int64_t  s_int_max = std::numeric_limits<int64_t>::max();

    public:

        random(int64_t custom_seed);
        bool init(int64_t custom_seed);

        void seed(int64_t seed);
        uint64_t seed();
        // Returns a random floating point value between 0 and 1.
        float get_float();
        // Returns a random uint32 value between a given range [min,max]
        // default values are int64_t min values and int64_t max value,
        // or between the specified range.
        // Cast the result to uint if you want all positive numbers
        int64_t get_int(int64_t min = 0, int64_t max = s_uint_max);

    private:
        uint64_t m_seed;
        bool m_initialized = false;
        std::mt19937 m_random_engine;
        std::uniform_int_distribution<uint64_t> m_distribution;
    };

    inline random::random(int64_t custom_seed)
        : m_seed(custom_seed)
        , m_random_engine(custom_seed)
    {
        m_random_engine.seed(custom_seed);
    }

    inline bool random::init(int64_t custom_seed)
    {
        if(m_initialized)
            return false;

        seed(custom_seed);
        m_initialized = true;
        return true;
    }

    inline void random::seed(int64_t seed)
    {
        m_seed = seed;
        std::mt19937 engine(seed);
        m_random_engine = engine;
    }

    inline uint64_t random::seed()
    {
        return m_seed;
    }

    inline float random::get_float()
    {
        return static_cast<float>(m_distribution(m_random_engine)) / static_cast<float>(s_uint_max);
    }

    inline int64_t random::get_int(int64_t min /*= s_uint_min*/, int64_t max /*= s_uint_max*/)
    {
        std::uniform_int_distribution<std::mt19937::result_type> distribution(min, max);
        return distribution(m_random_engine);
    }

}