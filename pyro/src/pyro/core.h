#pragma once

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define PYRO_PLATFORM_WIN
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define PYRO_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define PYRO_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define PYRO_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define PYRO_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection


// DLL support
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
#endif // End of DLL support

// 1 shifted by x places
// 1 << 0 = `0000 0001`
// 1 << 1 = `0000 0010`
// 1 << 2 = `0000 0100`
#define BIT(x) 1 << x

#if PYRO_DEBUG
    #define PYRO_ENABLE_ASSERTS
#endif

#ifdef PYRO_ENABLE_ASSERTS
    #define PYRO_ASSERT(expression, ...) { if(!(expression)) {PYRO_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
    #define PYRO_CORE_ASSERT(expression, ...) { if(!(expression)) {PYRO_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
    #define PYRO_ASSERT(expression, ...)
    #define PYRO_CORE_ASSERT(expression, ...)
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