#pragma once

#include <memory>

namespace Confuse {
	void initializeCore();
	void shutdownCore();
}


#ifdef CE_DEBUG
	#define CE_ENABLE_ASSERTS
#endif

#ifdef CE_PLATFORM_WINDOWS
	#ifdef CE_BUILD_DLL
		#define CONFUSE_API __declspec(dllexport)
	#else
		#define CONFUSE_API __declspec(dllimport)
	#endif
#else
	#error ConfuseEngine now just on windows platform!
#endif

#ifdef CE_ENABLE_ASSERTS
	#define CE_ASSERT(x, ...) {if(!(x)){CE_ERROR("assertation failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define CE_CORE_ASSERT(x, ...){if(!(x)){CE_CORE_ERROR("assertation failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define CE_ASSERT(x, ...)
	#define CE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define CE_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)