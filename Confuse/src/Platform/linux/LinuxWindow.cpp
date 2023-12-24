#include "CEpch.h"
#include "LinuxWindow.h"

#include <glad/glad.h>

#include "Confuse/Events/ApplicationEvent.h"
#include "Confuse/Events/MouseEvent.h"
#include "Confuse/Events/KeyEvent.h"

namespace Confuse{
    static bool s_glfwInitialized = false;

    static void GLFWErrorCallback(int error, const char* description){
        CE_CORE_ERROR("glfw error ({0}): {1}", error, description);
    }

    Window* Window::create(const WindowProps& props){
        return new LinuxWindow(props);
    }

    LinuxWindow::LinuxWindow(const WindowProps& props){
        init(props);
    }

    LinuxWindow::~LinuxWindow(){
        shutdown();
    }

    void LinuxWindow::init(const WindowProps& props){
        m_data.Title = props.Title;
        m_data.Width = props.Width;
        m_data.Height = props.Height;

        CE_CORE_INFO("creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if(!s_glfwInitialized){
            int success = glfwInit();
            CE_CORE_ASSERT(success, "could not init glfw");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_glfwInitialized = true;
        }

        m_window = glfwCreateWindow((int)props.Width, (int)props.Height, m_data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        CE_CORE_ASSERT(status, "failed to initialize glad!");
        glfwSetWindowUserPointer(m_window, &m_data);
        setVSync(true);

        // callbacks:
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            switch(action){
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch(action){
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });
    }

    void LinuxWindow::shutdown(){
        glfwDestroyWindow(m_window);
    }

    void LinuxWindow::onUpdate(){
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }

    void LinuxWindow::setVSync(bool enabled){
        if(enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_data.VSync = enabled;
    }

    bool LinuxWindow::isVSync() const{
        return m_data.VSync;
    }
}