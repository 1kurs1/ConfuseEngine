#pragma once

#include "core/ce_Device.hpp"
#include "window/ce_Window.hpp"

using namespace ConfuseEngine;

namespace ConfuseGraphicsCore{
    static void check_vk_result(VkResult err){
        if(err == 0) return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if(err < 0) abort();
    }

    class CE_Imgui{
    public:
        CE_Imgui(CE_Window& window, CE_Device& device, VkRenderPass renderPass, uint32_t imagesCount);
        ~CE_Imgui();

        void newFrame();
        void render(VkCommandBuffer commandBuffer);

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        void runExample();

    private:
        CE_Device& m_rDevice;
        
        VkDescriptorPool m_descriptorPool;
    };
}