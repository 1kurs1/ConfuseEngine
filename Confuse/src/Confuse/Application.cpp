#include "CEpch.h"
#include "Application.h"

#include <glad/glad.h>

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

        glGenVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        m_vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        uint32_t indices[3] = {0, 1, 2};
        m_indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_position;
            out vec3 v_position;

            void main(){
                v_position = a_position;
                gl_Position = vec4(a_position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;
            in vec3 v_position;

            void main(){
                color = vec4(v_position * 0.5 + 0.5, 1.0);
            }
        )";

        m_shader.reset(new Shader(vertexSrc, fragmentSrc));
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
            glClearColor(0.06, 0.06, 0.06, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_shader->bind();
            glBindVertexArray(m_vertexArray);
            glDrawElements(GL_TRIANGLES, m_indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

            for(Layer* layer: m_layerStack)
                layer->onUpdate();

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