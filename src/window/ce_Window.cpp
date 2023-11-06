#include "window/ce_Window.hpp"

namespace ConfuseGraphicsCore{
    CE_Window::CE_Window(int w, int h, std::string name) : m_width{w}, m_height{h}, m_windowName{name}{
        initWindow();
    }

    CE_Window::~CE_Window(){
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void CE_Window::initWindow(){
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_pWindow = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(m_pWindow, this);
        glfwSetFramebufferSizeCallback(m_pWindow, frameBufferResizedCallback);
    }

    void CE_Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface){
        if(glfwCreateWindowSurface(instance, m_pWindow, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void CE_Window::frameBufferResizedCallback(GLFWwindow* window, int width, int height){
        auto ceWindow = reinterpret_cast<CE_Window*>(glfwGetWindowUserPointer(window));
        ceWindow->m_framebufferResized = true;
        ceWindow->m_width = width;
        ceWindow->m_height = height;
    }

}