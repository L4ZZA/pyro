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