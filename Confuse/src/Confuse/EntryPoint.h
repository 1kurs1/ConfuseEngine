#ifdef CE_PLATFORM_LINUX

extern Confuse::Application* Confuse::createApplication();

int main(int argc, char** argv){
    auto app = Confuse::createApplication();
    app->run();
    delete app;
}

#endif