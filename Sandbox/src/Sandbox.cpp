#include "Confuse/Application.h"
#include "Confuse/EntryPoint.h"

#include "Confuse/Image.h"
#include "Confuse/Timer.h"

#include "Renderer.h"

using namespace Confuse;

class ExampleLayer : public Confuse::Layer{
public:
    virtual void onUIRender() override{
        ImGui::Begin("Settings");
        ImGui::Text("last render: %.3fms", m_lastRenderTime);
        if(ImGui::Button("Render")){
            render();
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
        m_renderer.render();

        m_lastRenderTime = timer.elapsedMilis();
    }

private:
    Renderer m_renderer;
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