#include "CEpch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

#include "Confuse/Renderer/Renderer.h"

#include "Confuse/Log.h"
#include "Confuse/Input.h"

namespace Confuse{

    #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_instance = nullptr;

    Application::Application(){
        CE_CORE_ASSERT(!s_instance, "application already exists!");
        s_instance = this;

        m_window = std::unique_ptr<Window>(Window::create());
        m_window->setEventCallback(BIND_EVENT_FN(onEvent));

        m_imGuiLayer = new ImGuiLayer();
        pushOverlay(m_imGuiLayer);
    }

    Application::~Application(){}

    void Application::pushLayer(Layer* layer){
        m_layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* overlay){
        m_layerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    void Application::onEvent(Event& e){
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
        
        for(auto it = m_layerStack.end(); it != m_layerStack.begin();){
            (*--it)->onEvent(e);
            if(e.handled) break;
        }
    }

    void Application::run(){
        while(m_running){
            float time = (float)glfwGetTime();
            Timestep timestep = time - m_lastFrameTime;
            m_lastFrameTime = time;

            for(Layer* layer: m_layerStack)
                layer->onUpdate(timestep);

            m_imGuiLayer->begin();
            for(Layer* layer: m_layerStack)
                layer->onImGuiRender();
            m_imGuiLayer->end();

            m_window->onUpdate();
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& e){
        m_running = false;
        return true;
    }
}