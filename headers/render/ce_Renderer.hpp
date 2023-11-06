#pragma once

#include "window/ce_Window.hpp"
#include "core/ce_Device.hpp"
#include "render/ce_SwapChain.hpp"

using namespace ConfuseEngine;

namespace ConfuseEngineRenderer{
    class CE_Renderer{
    public:
        CE_Renderer(ConfuseGraphicsCore::CE_Window& window, CE_Device& device);
        ~CE_Renderer();

        CE_Renderer(const CE_Renderer&) = delete;
        CE_Renderer &operator=(const CE_Renderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const {return m_pSwapChain->getRenderPass();}
        float getAspectRatio() const {return m_pSwapChain->extentAspectRatio();}
        bool isFrameInProgress() const {return m_isFrameStarted;}
        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(m_isFrameStarted && "cannot get command buffer when frame not is grogress!");
            return m_commandBuffers[m_currentFrameIndex];
        }

        int getFrameIndex() const{
            assert(m_isFrameStarted && "cannot get frame index when frame not is grogress!");
            return m_currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();    
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        ConfuseGraphicsCore::CE_Window& m_rWindow;
        CE_Device& m_rDevice;
        std::unique_ptr<CE_SwapChain> m_pSwapChain;
        std::vector<VkCommandBuffer> m_commandBuffers;

        uint32_t m_currentImageIndex;
        int m_currentFrameIndex{0};
        bool m_isFrameStarted{false};
    };
}