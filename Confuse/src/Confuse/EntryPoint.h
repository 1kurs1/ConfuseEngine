#pragma once

#include "Core.h"

#ifdef CE_PLATFORM_WINDOWS

extern Confuse::Application* Confuse::createApplication();

int main(int argc, char** argv) {
	Confuse::Log::init();

	CE_CORE_INFO("Confuse Engine initialization");

	auto app = Confuse::createApplication();
	app->run();
	delete app;
}
#endif