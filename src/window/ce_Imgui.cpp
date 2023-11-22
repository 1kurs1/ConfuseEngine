#include "window/ce_Imgui.hpp"

namespace ConfuseGraphicsCore{
    CE_Imgui::CE_Imgui(CE_Window& window, CE_Device& device, VkRenderPass renderPass, uint32_t imageCount) : m_rDevice(device){
         VkDescriptorPoolSize poolSizes[] = {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
        poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
        poolInfo.pPoolSizes = poolSizes;
        if(vkCreateDescriptorPool(device.device(), &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS){
            throw std::runtime_error("failed to set up descriptor pool for imgui!");
        }
        
        // imgui setup:
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // style:
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForVulkan(window.getGLFWwindow(), true);
        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = device.getInstance();
        initInfo.PhysicalDevice = device.getPhysicalDevice();
        initInfo.Device = device.device();
        initInfo.QueueFamily = device.getGraphicsQueueFamily();
        initInfo.Queue = device.graphicsQueue();

        initInfo.PipelineCache = VK_NULL_HANDLE;
        initInfo.DescriptorPool = m_descriptorPool;

        initInfo.Allocator = VK_NULL_HANDLE;
        initInfo.MinImageCount = 2;
        initInfo.ImageCount = imageCount;
        initInfo.CheckVkResultFn  = check_vk_result;
        ImGui_ImplVulkan_Init(&initInfo, renderPass);

        auto commandBuffer = device.beginSingleTimeCommands();
        ImGui_ImplVulkan_CreateFontsTexture();
        device.endSingleTimeCommands(commandBuffer);
        ImGui_ImplVulkan_DestroyFontsTexture();
    }

    CE_Imgui::~CE_Imgui(){
        vkDestroyDescriptorPool(m_rDevice.device(), m_descriptorPool, nullptr);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void CE_Imgui::newFrame(){
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void CE_Imgui::render(VkCommandBuffer commandBuffer){
        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer);
    }

    void CE_Imgui::runExample(){
        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Confuse Engine");

            ImGui::Text("Nicromis");
            ImGui::Checkbox("Something", &show_demo_window); 
            ImGui::Checkbox("Another something", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float *)&clear_color); 

            if (ImGui::Button("Button"))  
            counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        if (show_another_window) {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Nicromis Editor");
            if (ImGui::Button("Close")) show_another_window = false;
            ImGui::End();
        }
    }
}