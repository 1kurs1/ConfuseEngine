#pragma once

#include "Confuse/Layer.h"

#include "Confuse/Events/ApplicationEvent.h"
#include "Confuse/Events/KeyEvent.h"
#include "Confuse/Events/MouseEvent.h"

namespace Confuse{
    class ImGuiLayer : public Layer{
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void onAttach();
        void onDetach();
        void onUpdate();
        void onEvent(Event& event);

    private:
        bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool onMouseMovedEvent(MouseMovedEvent& e);
        bool onMouseScrolledEvent(MouseScrolledEvent& e);
        bool onKeyPressedEvent(KeyPressedEvent& e);
        bool onKeyReleasedEvent(KeyReleasedEvent& e);
        bool onKeyTypedEvent(KeyTypedEvent& e);
        bool onWindowResizedEvent(WindowResizeEvent& e);

    private:
        float m_time = 0.0f;
    };
}