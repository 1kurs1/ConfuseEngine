#pragma once

#include "Core.h"

#include "Window.h"
#include "Confuse/LayerStack.h"
#include "Confuse/Events/Event.h"
#include "Confuse/Events/ApplicationEvent.h"

#include "Confuse/Core/Timestep.h"

#include "Confuse/ImGui/ImGuiLayer.h"

namespace Confuse{
    class Application{
    public:
        Application();
        virtual ~Application();

        void run();
        void onEvent(Event& e);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);
        
        inline Window& getWindow() {return *m_window;}
        inline static Application& get() {return *s_instance;}

    private:
        bool onWindowClose(WindowCloseEvent& e);

    private:
        std::unique_ptr<Window> m_window;
        ImGuiLayer* m_imGuiLayer;
        bool m_running = true;
        LayerStack m_layerStack;
        float m_lastFrameTime = 0.0f;

    private:
        static Application* s_instance;
    };

    // defined in client
    Application* createApplication();
}