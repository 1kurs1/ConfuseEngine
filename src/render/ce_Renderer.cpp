#include "render/ce_Renderer.hpp"

namespace ConfuseEngineRenderer{
    CE_Renderer::CE_Renderer(ConfuseGraphicsCore::CE_Window& window, CE_Device& device) : m_rWindow{window}, m_rDevice{device}{
        recreateSwapChain();
        createCommandBuffers();
    }

    CE_Renderer::~CE_Renderer(){freeCommandBuffers();}

    void CE_Renderer::recreateSwapChain(){
        auto extent = m_rWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = m_rWindow.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(m_rDevice.device());

        if (m_pSwapChain == nullptr) {
            m_pSwapChain = std::make_unique<CE_SwapChain>(m_rDevice, extent);
        } else {
            std::shared_ptr<CE_SwapChain> oldSwapChain = std::move(m_pSwapChain);
            m_pSwapChain = std::make_unique<CE_SwapChain>(m_rDevice, extent, oldSwapChain);

            if (!oldSwapChain->compareSwapFormats(*m_pSwapChain.get())) {
                throw std::runtime_error("Swap chain image(or depth) format has changed!");
            }
        }
    }

    void CE_Renderer::createCommandBuffers(){
        m_commandBuffers.resize(CE_SwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_rDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

        if(vkAllocateCommandBuffers(m_rDevice.device(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void CE_Renderer::freeCommandBuffers(){
        vkFreeCommandBuffers(m_rDevice.device(), m_rDevice.getCommandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
        m_commandBuffers.clear();   
    }

    VkCommandBuffer CE_Renderer::beginFrame(){
        assert(!m_isFrameStarted && "can't call beginFrame while already in progress!");

        auto result = m_pSwapChain->acquireNextImage(&m_currentImageIndex);
        
        if(result == VK_ERROR_OUT_OF_DATE_KHR){
            recreateSwapChain();
            return nullptr;
        }
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        m_isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS){
            throw std::runtime_error("failed to begin command buffer!");
        }

        return commandBuffer;
    }

    void CE_Renderer::endFrame(){
        assert(m_isFrameStarted && "can't call endFrame while frame is not in progress");
        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = m_pSwapChain->submitCommandBuffers(&commandBuffer, &m_currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            m_rWindow.wasWindowResized()) {
            m_rWindow.resetWindowResizedFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        m_isFrameStarted = false;
        m_currentFrameIndex = (m_currentFrameIndex + 1) % CE_SwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void CE_Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer){
        assert(m_isFrameStarted && "can't call beginSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "can't begin render pass on command buffer from a different frame!");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_pSwapChain->getRenderPass();
        renderPassInfo.framebuffer = m_pSwapChain->getFrameBuffer(m_currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_pSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValue{};
        clearValue[0].color = {0.0f, 0.0f, 0.01f, 1.0f};
        clearValue[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValue.size());
        renderPassInfo.pClearValues = clearValue.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_pSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(m_pSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, m_pSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    }

    void CE_Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer){
        assert(m_isFrameStarted && "can't call endSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "can't end render pass on command buffer from a different frame!");

        vkCmdEndRenderPass(commandBuffer);
    }

}