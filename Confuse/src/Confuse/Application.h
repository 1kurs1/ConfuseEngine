#pragma once

#include "Core.h"
#include "Events/Event.h"

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