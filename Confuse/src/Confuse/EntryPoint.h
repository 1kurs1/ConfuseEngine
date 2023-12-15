#ifdef CE_PLATFORM_LINUX
#include "Log.h"

extern Confuse::Application* Confuse::createApplication();

int main(int argc, char** argv){
    Confuse::Log::init();
    Confuse::Log::getCoreLogger()->info("initialization core");
    Confuse::Log::getClientLogger()->info("init application");

    auto app = Confuse::createApplication();
    app->run();
    delete app;
}

#endif