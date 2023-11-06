#pragma once

#include "core/ce_Device.hpp"

namespace ConfuseEngineRenderer {

    class CE_SwapChain {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        CE_SwapChain(ConfuseEngine::CE_Device &deviceRef, VkExtent2D windowExtent);
        CE_SwapChain(ConfuseEngine::CE_Device &deviceRef, VkExtent2D windowExtent, std::shared_ptr<CE_SwapChain> previous);
        ~CE_SwapChain();

        CE_SwapChain(const CE_SwapChain &) = delete;
        CE_SwapChain& operator=(const CE_SwapChain &) = delete;

        VkFramebuffer getFrameBuffer(int index) { return m_swapChainFramebuffers[index]; }
        VkRenderPass getRenderPass() { return m_renderPass; }
        VkImageView getImageView(int index) { return m_swapChainImageViews[index]; }
        size_t imageCount() { return m_swapChainImages.size(); }
        VkFormat getSwapChainImageFormat() { return m_swapChainImageFormat; }
        VkExtent2D getSwapChainExtent() { return m_swapChainExtent; }
        uint32_t width() { return m_swapChainExtent.width; }
        uint32_t height() { return m_swapChainExtent.height; }

        float extentAspectRatio() {
            return static_cast<float>(m_swapChainExtent.width) / static_cast<float>(m_swapChainExtent.height);
        }
        VkFormat findDepthFormat();

        VkResult acquireNextImage(uint32_t *imageIndex);
        VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

        bool compareSwapFormats(const CE_SwapChain& swapChain) const {
            return swapChain.m_swapChainDepthFormat == m_swapChainDepthFormat && swapChain.m_swapChainImageFormat == m_swapChainImageFormat;
        }

    private:
        void init();
        void createSwapChain();
        void createImageViews();
        void createDepthResources();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        VkFormat m_swapChainImageFormat;
        VkFormat m_swapChainDepthFormat;
        VkExtent2D m_swapChainExtent;

        std::vector<VkFramebuffer> m_swapChainFramebuffers;
        VkRenderPass m_renderPass;

        std::vector<VkImage> m_depthImages;
        std::vector<VkDeviceMemory> m_depthImageMemorys;
        std::vector<VkImageView> m_depthImageViews;
        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;

        ConfuseEngine::CE_Device &m_rDevice;
        VkExtent2D m_windowExtent;

        VkSwapchainKHR m_swapChain;
        std::shared_ptr<CE_SwapChain> m_oldSwapChain;

        std::vector<VkSemaphore> m_imageAvailableSemaphores;
        std::vector<VkSemaphore> m_renderFinishedSemaphores;
        std::vector<VkFence> m_inFlightFences;
        std::vector<VkFence> m_imagesInFlight;
        size_t m_currentFrame = 0;
    };
}