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