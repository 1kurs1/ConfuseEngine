#pragma once
#include <signal.h>

#ifdef CE_PLATFORM_WINDOWS
// windows sucks...
    #ifdef CE_BUILD_LIB
        #define CONFUSE_API __declspec(dllexport)
    #else
        #define CONFUSE_API __declspec(dllimport)
    #endif
#elif CE_PLATFORM_LINUX
    #ifdef CE_BUILD_LIB
        #define CONFUSE_API
    #endif
#else
#endif

#ifdef CE_ENABLE_ASSERTS
    #define CE_ASSERT(x, ...) { if(!(x)) { CE_ERROR("assertion failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
	#define CE_CORE_ASSERT(x, ...) { if(!(x)) { CE_CORE_ERROR("assertion failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
#else
    #define CE_ASSERT(x, ...)
	#define CE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1<<x)
#define CE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)