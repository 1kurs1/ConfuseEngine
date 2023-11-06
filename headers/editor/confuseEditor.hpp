#pragma once

#include "core/ce_Camera.hpp"
#include "core/ce_Descriptors.hpp"
#include "window/ce_Window.hpp"
#include "core/ce_Device.hpp"
#include "render/ce_Renderer.hpp"
#include "render/ce_GameObject.hpp"
#include "input/keyboardMovementController.hpp"

using namespace ConfuseEngine;
using namespace ConfuseEngineRenderer;
using namespace ConfuseEngineInput;

namespace ConfuseGraphicsCore{
    class Editor{
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        Editor();
        ~Editor();

        Editor(const Editor&) = delete;
        Editor &operator=(const Editor&) = delete;

        void startEditor();
    private:
        void loadGameObjects();

        CE_Window m_window{WIDTH, HEIGHT, "ConfuseEditor"};
        CE_Device m_device{m_window};
        CE_Renderer m_renderer{m_window, m_device};

        std::unique_ptr<CE_DescriptorPool> m_pGlobalPool{};
        CE_GameObject::Map m_gameObjects;
    };
}