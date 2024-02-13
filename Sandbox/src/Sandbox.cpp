#include "Confuse/Application.h"

#include <glm/gtc/type_ptr.hpp>

#include "Confuse/EntryPoint.h"

#include "Confuse/Image.h"
#include "Confuse/Timer.h"

#include "Renderer.h"
#include "Camera.h"

using namespace Confuse;

class ExampleLayer : public Confuse::Layer{
public:
    ExampleLayer() : m_camera(45.0f, 0.1f, 100.0f){
        Material& redSphere = m_scene.materials.emplace_back();
        redSphere.albedo = {1.0f, 0.0f, 1.0f};
        redSphere.roughness = 0.0f;

        Material& blueSphere = m_scene.materials.emplace_back();
        blueSphere.albedo = {0.2f, 0.3f, 1.0f};
        blueSphere.roughness = 0.1f;

        Material& orangeSphere = m_scene.materials.emplace_back();
        orangeSphere.albedo = {0.8f, 0.5f, 0.2f};
        orangeSphere.roughness = 0.1f;
        orangeSphere.emissionColor = orangeSphere.albedo;
        orangeSphere.emissionPower = 2.0f;

        {
            Sphere sphere;
            sphere.position = {0.0f, 0.0f, 0.0f};
            sphere.radius = 1.0f;
            sphere.materialIndex = 0;

            m_scene.spheres.push_back(sphere);
        }

        {
            Sphere sphere;
            sphere.position = {2.0f, 0.0f, 0.0f};
            sphere.radius = 1.0f;
            sphere.materialIndex = 2;

            m_scene.spheres.push_back(sphere);
        }

        {
            Sphere sphere;
            sphere.position = {0.0f, -101.0f, 0.0f};
            sphere.radius = 100.0f;
            sphere.materialIndex = 1;

            m_scene.spheres.push_back(sphere);
        }
    }

    virtual void onUpdate(float ts) override{
        if(m_camera.onUpdate(ts))
            m_renderer.resetFrameIndex();
    }

    virtual void onUIRender() override{
        ImGui::Begin("Settings");
        ImGui::Text("last render: %.3fms", m_lastRenderTime);
        if(ImGui::Button("render")){
            render();
        }

        ImGui::Checkbox("accumulate", &m_renderer.getSettings().accumulate);
        ImGui::Checkbox("slow random", &m_renderer.getSettings().slowRandom);

        if(ImGui::Button("reset"))
            m_renderer.resetFrameIndex();

        ImGui::End();

        ImGui::Begin("Scene");
        for(size_t i = 0; i < m_scene.spheres.size(); i++){
            ImGui::PushID(i);

            Sphere& sphere = m_scene.spheres[i];
            ImGui::DragFloat3("position", glm::value_ptr(sphere.position), 0.1f);
            ImGui::DragFloat("radius", &sphere.radius, 0.1f);
            ImGui::DragInt("material", &sphere.materialIndex, 1.0f, 0, (int)m_scene.materials.size() - 1);

            ImGui::Separator();

            ImGui::PopID();
        }

        for(size_t i = 0; i < m_scene.materials.size(); i++){
            ImGui::PushID(i);

            Material& material = m_scene.materials[i];
            ImGui::ColorEdit3("albedo", glm::value_ptr(material.albedo));
            ImGui::DragFloat("roughness", &material.roughness, 0.05f, 0.0f, 1.0f);
            ImGui::DragFloat("metallic", &material.metallic, 0.05f, 0.0f, 1.0f);
            ImGui::ColorEdit3("emission color", glm::value_ptr(material.emissionColor));
            ImGui::DragFloat("emission power", &material.emissionPower, 0.05f, 0.0f, FLT_MAX);

            ImGui::Separator();

            ImGui::PopID();
        }

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");

        m_viewportWidth = ImGui::GetContentRegionAvail().x;
        m_viewportHeight = ImGui::GetContentRegionAvail().y;

        auto image = m_renderer.getFinalImage();
        if(image)
            ImGui::Image(image->getDescriptorSet(), {(float)image->getWidth(), (float)image->getHeight()}, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
        ImGui::PopStyleVar();

        render();
    }

    void render(){
        Timer timer;

        m_renderer.onResize(m_viewportWidth, m_viewportHeight);
        m_camera.onResize(m_viewportWidth, m_viewportHeight);
        m_renderer.render(m_scene, m_camera);

        m_lastRenderTime = timer.elapsedMilis();
    }

private:
    Renderer m_renderer;
    Camera m_camera;
    Scene m_scene;
    uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

    float m_lastRenderTime = 0.0f;
};

Confuse::Application* Confuse::createApplication(){
    Confuse::ApplicationSpecification spec;
    spec.name = "Confuse Engine";

    Confuse::Application* app = new Confuse::Application(spec);
    app->pushLayer<ExampleLayer>();
    app->setMenubarCallback([app](){
        if(ImGui::BeginMenu("File")){
            if(ImGui::MenuItem("Exit"))
                app->close();
            ImGui::EndMenu();
        }
    });

    return app;
}