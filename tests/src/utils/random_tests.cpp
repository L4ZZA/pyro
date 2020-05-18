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

BOOST_AUTO_TEST_CASE(seeded_random_float, *utf::tolerance(0.00001))
{
    random::init(1);
    float first = random::get_float();
    random::seed(2);
    float sedond = random::get_float();
    random::seed(1);
    float twin = random::get_float();

    BOOST_TEST(first == twin); // irrelevant difference
}

BOOST_AUTO_TEST_CASE(seeded_random_float_array)
{
    const int size = 100;
    std::array<float, size> first_array;
    std::array<float, size> second_array;
    std::array<float, size> twin_array;

    random::init(1);
    for (int i = 0; i < size; i++)
        first_array[i] = random::get_float();

    std::array<int, 2> seeds{ 2,1 };
    int repetitions = 4;
    for (int r = 0; r < repetitions; r++)
    {
        random::seed(2);
        for (int i = 0; i < size; i++)
            second_array[i] = random::get_float();
        //float sedond = random::get_float();
        random::seed(1);
        for (int i = 0; i < size; i++)
        {
            twin_array[i] = random::get_float();
            BOOST_TEST(first_array[i] == twin_array[i]); 
            BOOST_CHECK(first_array[i] == twin_array[i]); 
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()