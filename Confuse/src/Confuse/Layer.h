#pragma once

#include "Confuse/Core.h"
#include "Confuse/Events/Event.h"
#include "Confuse/Core/Timestep.h"

namespace Confuse{
    class Layer{
    public:
        Layer(const std::string& name = "layer");
        virtual ~Layer();

        virtual void onAttach(){}
        virtual void onDetach(){}
        virtual void onUpdate(Timestep ts) {}
        virtual void onImGuiRender(){}
        virtual void onEvent(Event& event){}

        inline const std::string& getName() const {return m_debugName;}

    protected:
        std::string m_debugName;
    };
}