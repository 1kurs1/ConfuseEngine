#pragma once

#include "core/stdinclude.hpp"

namespace ConfuseGraphicsCore{
    class CE_Window {
    public:
        CE_Window(int w, int h, std::string name);
        ~CE_Window();

        CE_Window(const CE_Window &) = delete;
        CE_Window &operator=(const CE_Window &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(m_pWindow); }
        VkExtent2D getExtent() { return {static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)}; }
        bool wasWindowResized() { return m_framebufferResized; }
        void resetWindowResizedFlag() { m_framebufferResized = false; }
        GLFWwindow *getGLFWwindow() const { return m_pWindow; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
        void initWindow();

        int m_width;
        int m_height;
        bool m_framebufferResized = false;

        std::string m_windowName;
        GLFWwindow *m_pWindow;
    };
}