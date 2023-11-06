#pragma once

#include "core/ce_Camera.hpp"
#include "render/ce_GameObject.hpp"

namespace ConfuseEngineRenderer{
    struct FrameInfo{
        int framIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        ConfuseEngine::CE_Camera& camera;
        VkDescriptorSet globalDescriptorSet;
        CE_GameObject::Map& rGameObjects;
    };
}