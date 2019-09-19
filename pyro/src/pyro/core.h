#pragma once
// all common Pyro definitions

#ifdef PYRO_PLATFORM_WIN

#if PYRO_DYNAMIC
    #ifdef PYRO_BUILD_DLL
        #define PYRO_API __declspec(dllexport)
    #else
        #define PYRO_API __declspec(dllimport)
    #endif
#else
    #define PYRO_API
#endif

#else
    #error Pyro only supports Windows!
#endif

// 1 shifted by x places
// 1 << 0 = `0000 0001`
// 1 << 1 = `0000 0010`
// 1 << 2 = `0000 0100`
#define BIT(x) 1 << x

#if PYRO_DEBUG
    #define PYRO_ENABLE_ASSERTS
#endif

#ifdef PYRO_ENABLE_ASSERTS

    #define PYRO_ASSERT_NO_MESSAGE(condition) { if(!(condition)) { PYRO_ERROR("Assertion Failed!"); __debugbreak(); } }
    #define PYRO_ASSERT_MESSAGE(condition, ...) { if(!(condition)) { PYRO_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

    #define PYRO_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro

    #define PYRO_ASSERT(...) PYRO_ASSERT_RESOLVE(__VA_ARGS__, PYRO_ASSERT_MESSAGE, PYRO_ASSERT_NO_MESSAGE)(__VA_ARGS__)
    #define PYRO_CORE_ASSERT(...) PYRO_ASSERT_RESOLVE(__VA_ARGS__, PYRO_ASSERT_MESSAGE, PYRO_ASSERT_NO_MESSAGE)(__VA_ARGS__)

#else
    #define PYRO_ASSERT(x, ...)
    #define PYRO_CORE_ASSERT(x, ...)
#endif

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#include <memory>
namespace pyro
{
    /// \brief safe and scoped pointer to an object. [Like unique_ptr]
    template<typename T>
    using scope = std::unique_ptr<T>;

    /// \brief ref counted pointer to an object. [Usually used for resources]
    template<typename T>
    using ref = std::shared_ptr<T>;
}