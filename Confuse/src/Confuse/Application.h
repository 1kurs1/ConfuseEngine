#pragma once

#include "Core.h"

#include "Window.h"
#include "Confuse/LayerStack.h"
#include "Confuse/Events/Event.h"
#include "Confuse/Events/ApplicationEvent.h"

namespace Confuse{
    class Application{
    public:
        Application();
        virtual ~Application();

        void run();
        void onEvent(Event& e);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);
        
    private:
        bool onWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_window;
        bool m_running = true;
        LayerStack m_layerStack;
    };

    // defined in client
    Application* createApplication();
}