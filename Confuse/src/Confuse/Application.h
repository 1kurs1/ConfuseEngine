#pragma once

#include "Core.h"

namespace Confuse{
    class Application{
    public:
        Application();
        virtual ~Application();

        void run();
    };

    // defined in client
    Application* createApplication();
}