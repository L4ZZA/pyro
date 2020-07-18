#include <boost/test/unit_test.hpp>
#include <array>
#include "utils/random.h"
#include "utils/noise.h"

namespace utf = boost::unit_test;
using namespace utils;

BOOST_AUTO_TEST_SUITE(noise_tests)

// checks that noise arrays with same seed contain the same values 
// even if random generator instances are different
BOOST_AUTO_TEST_CASE(same_rand_different_obj, *utf::tolerance(0.00001))
{
    const int size = 256;
    std::array<float, size> noise_2d;
    std::array<float, size> noise_2d_second;
    std::array<float, size> noise_2d_third;

    random rand(1);
    for (int i = 0; i < size; i++)
    {
        noise_2d[i] = rand.get_float();
    }

    random rand2(1);
    for (int i = 0; i < size; i++)
    {
        noise_2d_second[i] = rand2.get_float();
        BOOST_TEST(noise_2d[i] == noise_2d_second[i]);
    }

    random rand3(1);
    for (int i = 0; i < size; i++)
    {
        noise_2d_third[i] = rand3.get_float();
        BOOST_TEST(noise_2d[i] == noise_2d_third[i]);
    }
}

// checks that noise arrays with same seed contain the same values when updating
// the seed via the class method
BOOST_AUTO_TEST_CASE(same_rand_same_obj, *utf::tolerance(0.00001))
{
    const int size = 256;
    std::array<float, size> noise_2d;
    std::array<float, size> noise_2d_second;
    std::array<float, size> noise_2d_third;

    random rand(1);
    for (int i = 0; i < size; i++)
    {
        noise_2d[i] = rand.get_float();
    }
    rand.seed(2);
    for (int i = 0; i < size; i++)
    {
        noise_2d_second[i] = rand.get_float();
        BOOST_TEST(noise_2d[i] != noise_2d_second[i]);
    }

    rand.seed(1);
    for (int i = 0; i < size; i++)
    {
        noise_2d_third[i] = rand.get_float();
        BOOST_TEST(noise_2d[i] == noise_2d_third[i]);
    }
}

// checks that perlin_1d method with same parameters returns the same values
BOOST_AUTO_TEST_CASE(same_noise_1d)
{
    const int size = 256;
    std::array<float, size> noise_1d;
    std::array<float, size> noise_1d_second;

    utils::perlin_noise_1d(size, 5, 0.2f, 0, noise_1d.data());
    utils::perlin_noise_1d(size, 5, 0.2f, 0, noise_1d_second.data());

    for(int i = 0; i < size; i++)
    {
        BOOST_TEST(noise_1d[i] == noise_1d_second[i]);
    }
}

// checks that perlin_2d method with same parameters returns the same values
BOOST_AUTO_TEST_CASE(same_noise_2d)
{
    const int size = 256;
    std::array<float, size * size> noise_2d;
    std::array<float, size * size> noise_2d_second;

    utils::perlin_noise_2d(size, 5, 0.2f, 0, noise_2d.data());
    utils::perlin_noise_2d(size, 5, 0.2f, 0, noise_2d_second.data());

    for(int i = 0; i < size * size; i++)
    {
        BOOST_TEST(noise_2d[i] == noise_2d_second[i]);
    }
}

// checks that perlin_2d method with different parameters does not return the same values
BOOST_AUTO_TEST_CASE(noise_2d_changed)
{
    const int size = 256;
    std::array<float, size * size> noise_2d;
    std::array<float, size * size> noise_2d_second;

    utils::perlin_noise_2d(size, 5, 0.2f, 0, noise_2d.data());
    utils::perlin_noise_2d(size, 7, 0.3f, 0, noise_2d_second.data());

    for(int i = 0; i < size * size; i++)
    {
        BOOST_TEST(noise_2d[i] != noise_2d_second[i]);
    }

    utils::perlin_noise_2d(size, 5, 0.2f, 0, noise_2d_second.data());

    for(int i = 0; i < size * size; i++)
    {
        BOOST_TEST(noise_2d[i] == noise_2d_second[i]);
    }
}

BOOST_AUTO_TEST_SUITE_END()