#pragma once

#include "core/ce_Camera.hpp"
#include "render/ce_GameObject.hpp"

namespace ConfuseEngineRenderer{
    #define MAX_LIGHTS 2

    struct PointLight{
        glm::vec4 position{};
        glm::vec4 color{};      // rgb and intensity
    };

    struct GlobalUBO{
        glm::mat4 projection{1.f};
        glm::mat4 view{1.f};
        glm::mat4 inverseView{1.f};
        glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f};   // color and intensity
        PointLight pointLights[MAX_LIGHTS];
        int numLights;
    };

    struct FrameInfo{
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        ConfuseEngine::CE_Camera& camera;
        VkDescriptorSet globalDescriptorSet;
        CE_GameObject::Map& rGameObjects;
    };
}