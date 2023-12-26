#pragma once

#include "CEpch.h"

#include "Confuse/Core.h"
#include "Confuse/Events/Event.h"

namespace Confuse{
    struct WindowProps{
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Confuse Engine", unsigned int width = 1280, unsigned int height = 720) : Title(title), Width(width), Height(height){}
    };

    class Window{
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window(){}

        virtual void onUpdate() = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;

        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        virtual void* getNativeWindow() const = 0;

        static Window* create(const WindowProps& props = WindowProps());
    };
}