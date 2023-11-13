#include "render/RenderSystems/PointLightSystem.hpp"

namespace ConfuseEngineRenderer {

    struct PointLightPushConstants {
        glm::vec4 position{};
        glm::vec4 color{};
        float radius;
    };

    PointLightSystem::PointLightSystem(ConfuseEngine::CE_Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : m_rDevice{device} {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    PointLightSystem::~PointLightSystem() {
        vkDestroyPipelineLayout(m_rDevice.device(), m_pipelineLayout, nullptr);
    }

    void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PointLightPushConstants);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

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

    void PointLightSystem::createPipeline(VkRenderPass renderPass) {
        assert(m_pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        CE_Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        CE_Pipeline::enableAlphaBlending(pipelineConfig);

        pipelineConfig.attributeDescriptions.clear();
        pipelineConfig.bindingDescriptions.clear();
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = m_pipelineLayout;
        m_pPipeline = std::make_unique<CE_Pipeline>(m_rDevice,"shaders/pointLight.vert.spv", "shaders/pointLight.frag.spv", pipelineConfig);
    }

    void PointLightSystem::update(FrameInfo& frameInfo, GlobalUBO& ubo) {
        auto rotateLight = glm::rotate(glm::mat4(1.f), 0.5f * frameInfo.frameTime, {0.f, -1.f, 0.f});
        int lightIndex = 0;
        for (auto& kv : frameInfo.rGameObjects) {
            auto& obj = kv.second;
            if (obj.pointLight == nullptr) continue;

            assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

            obj.transform.translation = glm::vec3(rotateLight * glm::vec4(obj.transform.translation, 1.f));

            ubo.pointLights[lightIndex].position = glm::vec4(obj.transform.translation, 1.f);
            ubo.pointLights[lightIndex].color = glm::vec4(obj.color, obj.pointLight->lightIntensity);

            lightIndex += 1;
        }
        ubo.numLights = lightIndex;
    }

    void PointLightSystem::render(FrameInfo& frameInfo) {
        std::map<float, CE_GameObject::id_t> sorted;
        for (auto& kv : frameInfo.rGameObjects) {
            auto& obj = kv.second;
            if (obj.pointLight == nullptr) continue;

            auto offset = frameInfo.camera.getPosition() - obj.transform.translation;
            float disSquared = glm::dot(offset, offset);
            sorted[disSquared] = obj.getId();
        }

        m_pPipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
            auto& obj = frameInfo.rGameObjects.at(it->second);

            PointLightPushConstants push{};
            push.position = glm::vec4(obj.transform.translation, 1.f);
            push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
            push.radius = obj.transform.scale.x;

            vkCmdPushConstants(frameInfo.commandBuffer, m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PointLightPushConstants), &push);
            vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
        }
    }
}
