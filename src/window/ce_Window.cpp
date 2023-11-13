#include "window/ce_Window.hpp"

namespace ConfuseGraphicsCore {
    CE_Window::CE_Window(int w, int h, std::string name) : m_width{w}, m_height{h}, m_windowName{name} {
        initWindow();
    }

    CE_Window::~CE_Window() {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void CE_Window::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_pWindow = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(m_pWindow, this);
        glfwSetFramebufferSizeCallback(m_pWindow, framebufferResizeCallback);
    }

    void CE_Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, m_pWindow, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to craete window surface");
        }
    }

    void CE_Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto aWindow = reinterpret_cast<CE_Window*>(glfwGetWindowUserPointer(window));
        aWindow->m_framebufferResized = true;
        aWindow->m_width = width;
        aWindow->m_height = height;
    }
}