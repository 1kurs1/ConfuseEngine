#include "render/RenderSystems/TextureRenderSystem.hpp"

namespace ConfuseEngineRenderer {

    struct TexturePushConstantData {
        glm::mat4 modelMatrix{1.f};
        glm::mat4 normalMatrix{1.f};
    };

    TextureRenderSystem::TextureRenderSystem(ConfuseEngine::CE_Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : m_rDevice{device} {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    TextureRenderSystem::~TextureRenderSystem() {
        vkDestroyPipelineLayout(m_rDevice.device(), m_pipelineLayout, nullptr);
    }

    void TextureRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(TexturePushConstantData);

        m_renderSystemLayout = CE_DescriptorSetLayout::Builder(m_rDevice).addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT).build();

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout, m_renderSystemLayout->getDescriptorSetLayout()};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(m_rDevice.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void TextureRenderSystem::createPipeline(VkRenderPass renderPass) {
        assert(m_pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        CE_Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = m_pipelineLayout;
        m_pPipeline = std::make_unique<CE_Pipeline>(m_rDevice, "shaders/textureShader.vert.spv", "shaders/textureShader.frag.spv", pipelineConfig);
    }

    void TextureRenderSystem::renderGameObjects(FrameInfo& frameInfo) {
        m_pPipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        for (auto& kv : frameInfo.rGameObjects) {
            auto& obj = kv.second;

            if (obj.model == nullptr || obj.diffuseMap == nullptr) continue;

            auto imageInfo = obj.diffuseMap->getImageInfo();
            VkDescriptorSet descriptorSet1;
            CE_DescriptorWriter(*m_renderSystemLayout, frameInfo.frameDescriptorPool).writeImage(0, &imageInfo).build(descriptorSet1);

            vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 1, 1, &descriptorSet1, 0, nullptr);

            TexturePushConstantData push{};
            push.modelMatrix = obj.transform.mat4();
            push.normalMatrix = obj.transform.normalMatrix();

            vkCmdPushConstants(frameInfo.commandBuffer, m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(TexturePushConstantData), &push);

            obj.model->bind(frameInfo.commandBuffer);
            obj.model->draw(frameInfo.commandBuffer);
        }
    }
}