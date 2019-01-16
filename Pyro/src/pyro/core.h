#pragma once

// all common Pyro definitions

#ifdef PYRO_PLATFORM_WIN
    #ifdef PYRO_BUILD_DLL
        #define PYRO_API __declspec(dllexport)
    #else
        #define PYRO_API __declspec(dllimport)
    #endif
#else
    #error Pyro only supports Windows!
#endif

// 1 shifted by x places
// 1 << 0 = `0000 0001`
// 1 << 1 = `0000 0010`
// 1 << 2 = `0000 0100`
#define BIT(x) 1 << x


#ifdef PYRO_ENABLE_ASSERTS
    #define PYRO_ASSERT(x, ...) { if(!x) {PYRO_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
    #define PYRO_CORE_ASSERT(x, ...) { if(!x) {PYRO_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
    #define PYRO_ASSERT(x, ...)
    #define PYRO_CORE_ASSERT(x, ...)
#endif

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)