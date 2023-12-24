#include <Confuse.h>

class ExampleLayer : public Confuse::Layer{
public:
    ExampleLayer() : Layer("example"){}

    void onUpdate() override{
        CE_INFO("ExampleLayer::Update");
    }

    void onEvent(Confuse::Event& event) override{
        CE_TRACE("{0}", event);
    }
};

class Sandbox : public Confuse::Application{
public:
    Sandbox(){
        pushLayer(new ExampleLayer());
        pushOverlay(new Confuse::ImGuiLayer());
    }

    ~Sandbox(){}
};

Confuse::Application* Confuse::createApplication(){
    return new Sandbox();
}