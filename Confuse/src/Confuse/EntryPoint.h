#pragma once

#include "Application.h"

#ifdef CE_PLATFORM_LINUX

extern Confuse::Application* Confuse::createApplication();
bool g_applicationRunning = true;

namespace Confuse {

	int Main(){
		while (g_applicationRunning){
			Confuse::Application* app = Confuse::createApplication();
			app->run();
			delete app;
		}

		return 0;
	}

}

#ifdef CE_DIST
#else

int main(){
	return Confuse::Main();
}

#endif
#endif