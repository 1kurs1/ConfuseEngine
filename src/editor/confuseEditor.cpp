#include "editor/confuseEditor.hpp"
#include "render/RenderSystems/DefaultRenderSystem.hpp"
#include "render/RenderSystems/PointLightSystem.hpp"
#include "render/RenderSystems/TextureRenderSystem.hpp"
#include "core/ce_Buffer.hpp"

namespace ConfuseGraphicsCore{
    Editor::Editor(){
        m_pGlobalPool = CE_DescriptorPool::Builder(m_device).setMaxSets(CE_SwapChain::MAX_FRAMES_IN_FLIGHT).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, CE_SwapChain::MAX_FRAMES_IN_FLIGHT).build();

        m_framePools.resize(CE_SwapChain::MAX_FRAMES_IN_FLIGHT);
        auto framePoolBuilder = CE_DescriptorPool::Builder(m_device).setMaxSets(1000).addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000).addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000).setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
        for (int i = 0; i < m_framePools.size(); i++) {
            m_framePools[i] = framePoolBuilder.build();
        }

        loadGameObjects();
    }

    Editor::~Editor(){}

    void Editor::startEditor(){
        std::vector<std::unique_ptr<CE_Buffer>> uboBuffers(CE_SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<CE_Buffer>(m_device, sizeof(GlobalUBO), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout = CE_DescriptorSetLayout::Builder(m_device).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS).build();

        std::vector<VkDescriptorSet> globalDescriptorSets(CE_SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            CE_DescriptorWriter(*globalSetLayout, *m_pGlobalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
        }

        DefaultRenderSystem defaultRenderSystem{m_device, m_renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        PointLightSystem pointLightSystem{m_device, m_renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        TextureRenderSystem textureRenderSystem{m_device, m_renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        CE_Camera camera{};

        auto viewerObject = CE_GameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!m_window.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(m_window.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = m_renderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

            if (auto commandBuffer = m_renderer.beginFrame()) {
                int frameIndex = m_renderer.getFrameIndex();
                m_framePools[frameIndex]->resetPool();
                FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex], *m_framePools[frameIndex],m_gameObjects};

                // update:
                GlobalUBO ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                ubo.inverseView = camera.getInverseView();
                pointLightSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render:
                m_renderer.beginSwapChainRenderPass(commandBuffer);

                textureRenderSystem.renderGameObjects(frameInfo);
                defaultRenderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);

                m_renderer.endSwapChainRenderPass(commandBuffer);
                m_renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(m_device.device());
    }

    // std::unique_ptr<CE_Model> createCubeModel(CE_Device& device, glm::vec3 offset) {
    //     CE_Model::Builder modelBuilder{};
    //     modelBuilder.vertices = {
    //         // left side
    //         {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
    //         {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
    //         {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
    //         {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
 
    //         // right side
    //         {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
    //         {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
    //         {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
    //         {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
        
    //         // top side
    //         {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    //         {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    //         {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    //         {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        
    //         // bottom side
    //         {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    //         {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
    //         {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
    //         {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        
    //         // nose side
    //         {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    //         {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    //         {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    //         {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        
    //         // tail side
    //         {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    //         {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    //         {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    //         {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
                
    //     };
    //     for (auto& v : modelBuilder.vertices) {
    //         v.position += offset;
    //     }

    //     modelBuilder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9, 12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};

    //     return std::make_unique<CE_Model>(device, modelBuilder);
    // }

    void Editor::loadGameObjects(){
        std::shared_ptr<CE_Model> model = CE_Model::createModelFromFile(m_device, "models/rifle2.obj");
        auto rifle = CE_GameObject::createGameObject();
        rifle.model = model;
        rifle.transform.translation = {-3.f, .0f, 0.f};
        rifle.transform.scale = {-1.f, -1.f, -1.f};
        m_gameObjects.emplace(rifle.getId(), std::move(rifle));

        model = CE_Model::createModelFromFile(m_device, "models/colored_cube.obj");
        auto cube = CE_GameObject::createGameObject();
        cube.model = model;
        cube.transform.translation = {3.f, .0f, 0.f};
        cube.transform.scale = {1.f, 1.f, 1.f};
        m_gameObjects.emplace(cube.getId(), std::move(cube));

        model = CE_Model::createModelFromFile(m_device, "models/quad.obj");
        std::shared_ptr<CE_Texture> marbleTexture = CE_Texture::createTextureFromFile(m_device, "../textures/grass.jpg");
        auto floor = CE_GameObject::createGameObject();
        floor.model = model;
        floor.diffuseMap = marbleTexture;
        floor.transform.translation = {0.f, 1.f, 0.f};
        floor.transform.scale = {10.f, 1.f, 10.f};
        m_gameObjects.emplace(floor.getId(), std::move(floor));

        std::vector<glm::vec3> lightColors{
            {.1f, .1f, 1.f},
            {.9f, .9f, .9f}
        };

        for (int i = 0; i < lightColors.size(); i++) {
            auto pointLight = CE_GameObject::makePointLight(0.5f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(glm::mat4(1.f), (i * glm::two_pi<float>()) / lightColors.size(), {0.f, -1.f, 0.f});
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -.5f, -1.f, 1.f));
            m_gameObjects.emplace(pointLight.getId(), std::move(pointLight));
        }
    }
}