#include <boost/test/unit_test.hpp>
#include <array>
#include "utils/random.h"
//
//#define BOOST_CHECK_REL_EQ_MESSAGE_EXTENSION(L, R, M, CMP, ICMP, CT)         \
//    {                                                                       \
//        auto _1(L);                                                         \
//        auto _2(R);                                                         \
//        std::stringstream ss;                                               \
//        ss << "check " << BOOST_TEST_STRINGIZE(L) << " " << BOOST_TEST_STRINGIZE(CMP) << " " << BOOST_TEST_STRINGIZE(R) << " failed [" << _1 << " " << BOOST_TEST_STRINGIZE(ICMP) << " " << _2 << "] : " << M;\
//        BOOST_CHECK_IMPL( (_1 CMP _2), ss.str(), CT, CHECK_MSG );           \
//    }        
//#define BOOST_TEST_REL_EQ_MESSAGE_EXTENSION(L, R, M, CMP, ICMP, CT)         \
//    {                                                                       \
//        auto _1(L);                                                         \
//        auto _2(R);                                                         \
//        std::stringstream ss;                                               \
//        ss << "check " << BOOST_TEST_STRINGIZE(L) << " " << BOOST_TEST_STRINGIZE(CMP) << " " << BOOST_TEST_STRINGIZE(R) << " failed [" << _1 << " " << BOOST_TEST_STRINGIZE(ICMP) << " " << _2 << "] : " << M;\
//        BOOST_CHECK_IMPL( (_1 CMP _2), ss.str(), CT, CHECK_MSG );           \
//    }                                                                       \
///**/
//
//#define BOOST_CHECK_EQUAL_MESSAGE(L, R, M)      BOOST_TEST_REL_EQ_MESSAGE_EXTENSION(L, R, M, ==, !=, CHECK )
//#define BOOST_WARN_EQUAL_MESSAGE(L, R, M)       BOOST_TEST_REL_EQ_MESSAGE_EXTENSION(L, R, M, ==, !=, WARN )
//#define BOOST_REQUIRE_EQUAL_MESSAGE(L, R, M)    BOOST_TEST_REL_EQ_MESSAGE_EXTENSION(L, R, M, ==, !=, REQUIRE )

namespace utf = boost::unit_test;
using namespace utils;

BOOST_AUTO_TEST_SUITE(randoms)

BOOST_AUTO_TEST_CASE(random_values, *utf::tolerance(0.00001))
{
    float first;
    float second;
    float third;

    {
        random rand(1);
        first = rand.get_float();
    
        second = rand.get_float();
        BOOST_TEST(first != second);
    
        third = rand.get_float();
        BOOST_TEST(first  != third);
        BOOST_TEST(second != third);
    }
}

BOOST_AUTO_TEST_CASE(same_random_float, *utf::tolerance(0.00001))
{
    float first;
    float second;
    float third;

    {
        random rand(1);
        first = rand.get_float();
    }
    {
        random rand(1);
        second = rand.get_float();
    }
    {
        random rand(1);
        third = rand.get_float();
    }
    BOOST_TEST(first == second);
    BOOST_TEST(first == third);
}

BOOST_AUTO_TEST_CASE(random_float, *utf::tolerance(0.00001))
{
    random rand(1);
    float first = rand.get_float();
    rand.seed(2);
    float second = rand.get_float();
    rand.seed(1);
    float twin = rand.get_float();

    BOOST_TEST(first == twin); // irrelevant difference
}

BOOST_AUTO_TEST_CASE(random_float_array)
{
    random rand(1);
    const int size = 100;
    std::array<float, size> first_array;
    std::array<float, size> second_array;
    std::array<float, size> twin_array;

    for (int i = 0; i < size; i++)
        first_array[i] = rand.get_float();

    std::array<int, 2> seeds{ 2,1 };
    int repetitions = 4;
    for (int r = 0; r < repetitions; r++)
    {
        rand.seed(2);
        for (int i = 0; i < size; i++)
            second_array[i] = rand.get_float();
        //float second = rand.get_float();
        rand.seed(1);
        for (int i = 0; i < size; i++)
        {
            twin_array[i] = rand.get_float();
            BOOST_TEST(first_array[i] == twin_array[i]); 
            BOOST_CHECK(first_array[i] == twin_array[i]); 
        }
    }
}

BOOST_AUTO_TEST_CASE(random_int_range)
{
    random rand(1);
    int tries = 1000;
    int min = 2;
    int max = 10; 
    for(int i = 0; i < tries; i++)
    {
        int val = rand.get_int(min,max);
        BOOST_TEST((val >= min && val <= max));
        BOOST_TEST(!(val < min || val > max));
    }
}

BOOST_AUTO_TEST_SUITE_END()