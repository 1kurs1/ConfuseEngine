#pragma once

#include "core/ce_Camera.hpp"
#include "core/ce_Device.hpp"
#include "render/ce_FrameInfo.hpp"
#include "render/ce_GameObject.hpp"
#include "render/ce_Pipeline.hpp"

namespace ConfuseEngineRenderer {
    class PointLightSystem {
    public:
        PointLightSystem(ConfuseEngine::CE_Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PointLightSystem();

        PointLightSystem(const PointLightSystem &) = delete;
        PointLightSystem &operator=(const PointLightSystem &) = delete;

        void update(FrameInfo &frameInfo, GlobalUBO &ubo);
        void render(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        ConfuseEngine::CE_Device &m_rDevice;

        std::unique_ptr<CE_Pipeline> m_pPipeline;
        VkPipelineLayout m_pipelineLayout;
    };
}
