#pragma once

#include <GLFW/glfw3.h>

#include "Confuse/Window.h"
#include "Confuse/Renderer/GraphicsContext.h"

namespace Confuse{
    class LinuxWindow : public Window{
    public:
        LinuxWindow(const WindowProps& props);
        virtual ~LinuxWindow();

        void onUpdate() override;

        inline unsigned int getWidth() const override {return m_data.Width;}
        inline unsigned int getHeight() const override {return m_data.Height;}

        inline void setEventCallback(const EventCallbackFn& callback) override {m_data.EventCallback = callback;}
        void setVSync(bool enabled) override;
        bool isVSync() const override;

        inline virtual void* getNativeWindow() const {return m_window;}

    private:
        virtual void init(const WindowProps& props);
        virtual void shutdown();

    private:
        GLFWwindow* m_window;
        GraphicsContext* m_context;

        struct WindowData{
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_data;
    };
}