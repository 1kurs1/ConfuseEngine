#include "cepch.h"
#include "Base.h"

#include "Log.h"

// system parameters:
#define CONFUSE_BUILD_ID "v0.1.4"

namespace Confuse {
	void initializeCore() {
		Log::init();
		
		CE_CORE_TRACE("Confuse Engine {}", CONFUSE_BUILD_ID);
		CE_CORE_TRACE("initializing core...");
	}

	void shutdownCore() {
		CE_CORE_TRACE("shutting down...");
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		Confuse::initializeCore();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Confuse::shutdownCore();
		break;
	}
	return TRUE;
}