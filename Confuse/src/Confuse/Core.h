#pragma once

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

#define BIT(x) (1<<x)