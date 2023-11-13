#pragma once

#include "core/ce_Camera.hpp"
#include "core/ce_Descriptors.hpp"
#include "core/ce_Device.hpp"
#include "render/ce_FrameInfo.hpp"
#include "render/ce_GameObject.hpp"
#include "render/ce_Pipeline.hpp"

namespace ConfuseEngineRenderer {
    class TextureRenderSystem {
    public:
        TextureRenderSystem(ConfuseEngine::CE_Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~TextureRenderSystem();

        TextureRenderSystem(const TextureRenderSystem &) = delete;
        TextureRenderSystem &operator=(const TextureRenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        ConfuseEngine::CE_Device &m_rDevice;

        std::unique_ptr<CE_Pipeline> m_pPipeline;
        VkPipelineLayout m_pipelineLayout;

        std::unique_ptr<CE_DescriptorSetLayout> m_renderSystemLayout;
    };
}
