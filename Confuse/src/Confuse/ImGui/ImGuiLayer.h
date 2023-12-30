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

        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onImGuiRender() override;

        void begin();
        void end();

    private:
        float m_time = 0.0f;
    };
}