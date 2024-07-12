#pragma once

#ifdef CE_PLATFORM_WINDOWS

extern Confuse::Application* Confuse::createApplication();

int main(int argc, char** argv) {
	Confuse::Application* app = Confuse::createApplication();
	CE_CORE_ASSERT(app, "client application is null!");
	app->run();
	delete app;
}
#endif