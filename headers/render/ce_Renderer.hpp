#pragma once

#include "core/ce_Device.hpp"
#include "render/ce_SwapChain.hpp"
#include "window/ce_Window.hpp"

namespace ConfuseEngineRenderer {
    class CE_Renderer {
    public:
        CE_Renderer(CE_Window &window, ConfuseEngine::CE_Device &device);
        ~CE_Renderer();

        CE_Renderer(const CE_Renderer&) = delete;
        CE_Renderer &operator=(const CE_Renderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const { return m_pSwapChain->getRenderPass(); }
        float getAspectRatio() const { return m_pSwapChain->extentAspectRatio(); }
        bool isFrameInProgress() const { return m_isFrameStarted; }

        uint32_t getImageCount() const{return m_pSwapChain->imageCount();}

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(m_isFrameStarted && "cannot get command buffer when frame not in progress");
            return m_commandBuffers[m_currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(m_isFrameStarted && "cannot get frame index when frame not in progress");
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

        CE_Window &m_rWindow;
        ConfuseEngine::CE_Device &m_rDevice;
        std::unique_ptr<CE_SwapChain> m_pSwapChain;
        std::vector<VkCommandBuffer> m_commandBuffers;

        uint32_t m_currentImageIndex;
        int m_currentFrameIndex{0};
        bool m_isFrameStarted{false};
    };
}