#pragma once

#ifdef CE_PLATFORM_WINDOWS
	#ifdef CE_BUILD_DLL
		#define CONFUSE_API __declspec(dllexport)
	#else
		#define CONFUSE_API __declspec(dllimport)
	#endif
#else
#error Confuse now just on windows!
#endif

#ifdef CE_ENABLE_ASSERTS
	#define CE_ASSERT(x, ...){if(!(x)){CE_ERROR("assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define CE_CORE_ASSERT(x, ...) {if(!(x)){CE_CORE_ERROR("assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define CE_ASSERT(x, ...)
	#define CE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)