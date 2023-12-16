#ifdef CE_PLATFORM_LINUX
#include "Log.h"

extern Confuse::Application* Confuse::createApplication();

int main(int argc, char** argv){
    Confuse::Log::init();
    CE_CORE_INFO("initialization core");
    CE_INFO("init application");

    auto app = Confuse::createApplication();
    app->run();
    delete app;
}

#endif