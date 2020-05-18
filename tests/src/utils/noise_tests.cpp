#include <boost/test/unit_test.hpp>
#include <array>
#include "utils/random.h"
#include "utils/noise.h"

namespace utf = boost::unit_test;
using namespace utils;

BOOST_AUTO_TEST_SUITE(noise_tests)

BOOST_AUTO_TEST_CASE(noise_1d)
{
    const int size = 256;
    std::array<float, size> noise_2d;
    std::array<float, size> noise_2d_second;

    random::init(1);
    random::seed(1);
    for (int i = 0; i < size; i++)
    {
        noise_2d[i] = random::get_float();
    }
    random::seed(2);
    for (int i = 0; i < size; i++)
    {
        noise_2d_second[i] = random::get_float();
        BOOST_TEST(noise_2d[i] != noise_2d_second[i]);
    }    
    random::seed(1);
    for (int i = 0; i < size; i++)
    {
        noise_2d_second[i] = random::get_float();
        BOOST_TEST(noise_2d[i] == noise_2d_second[i]);
    }
}

BOOST_AUTO_TEST_CASE(noise_2d)
{
    const int size = 256;
    std::array<float, size * size> noise_2d;
    std::array<float, size * size> noise_2d_second;

    random::init(1);
    random::seed(1);
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
        {
            uint32_t i = y * size + x;
            noise_2d[i] = random::get_float();
        }

    random::seed(2);
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
        {
            uint32_t i = y * size + x;
            noise_2d_second[i] = random::get_float();
            BOOST_TEST(noise_2d[i] != noise_2d_second[i]);
        }

    random::seed(1);
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
        {
            uint32_t i = y * size + x;
            noise_2d_second[i] = random::get_float();
            BOOST_TEST(noise_2d[i] == noise_2d_second[i]);
        }
}

BOOST_AUTO_TEST_SUITE_END()