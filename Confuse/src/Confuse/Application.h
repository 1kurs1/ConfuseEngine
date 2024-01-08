#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "imgui.h"
#include <vulkan/vulkan.h>

#include "Layer.h"

void check_vk_result(VkResult err);

struct GLFWwindow;

namespace Confuse{
    struct ApplicationSpecification{
        std::string name = "Confuse Application";
        uint32_t width = 1600;
        uint32_t height = 900;
    };

    class Application{
    public:
        Application(const ApplicationSpecification& applicationSpecification = ApplicationSpecification());
        ~Application();

        static Application& get();

        void run();
        void setMenubarCallback(const std::function<void()>& menubarCallback){m_menubarCallback = menubarCallback;}

        template<typename T>
        void pushLayer(){
            static_assert(std::is_base_of<Layer, T>::value, "pushed type is not subclass of layer!");
            m_layerStack.emplace_back(std::make_shared<T>())->onAttach();
        }

        void pushLayer(const std::shared_ptr<Layer>& layer){
            m_layerStack.emplace_back(layer);
            layer->onAttach();
        }

        void close();

        float getTime();
        GLFWwindow* getWindowHandle() const {return m_windowHandle;}

        static VkInstance getInstance();
        static VkPhysicalDevice getPhysicalDevice();
        static VkDevice getDevice();

        static VkCommandBuffer getCommandBuffer(bool begin);
        static void flushCommandBuffer(VkCommandBuffer commandBuffer);

        static void submitResourceFree(std::function<void()>&& func);

    private:
        void init();
        void shutdown();

    private:
        ApplicationSpecification m_specification;
        GLFWwindow* m_windowHandle = nullptr;
        bool m_running = false;

        float m_timeStep = 0.0f;
        float m_frameTime = 0.0f;
        float m_lastFrameTime = 0.0f;

        std::vector<std::shared_ptr<Layer>> m_layerStack;
        std::function<void()> m_menubarCallback;
    };

    Application* createApplication();
}