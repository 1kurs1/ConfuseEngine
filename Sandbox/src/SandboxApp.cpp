#include <Confuse.h>

class ExampleLayer : public Confuse::Layer{
public:
    ExampleLayer() : Layer("example"){}

    void onUpdate() override{
        if(Confuse::Input::isKeyPressed(CE_KEY_TAB))
            CE_TRACE("tab key is pressed (poll)");
    }

    void onEvent(Confuse::Event& event) override{
        if(event.getEventType() == Confuse::EventType::KeyPressed){
            Confuse::KeyPressedEvent& e = (Confuse::KeyPressedEvent&)event;
            if(e.getKeyCode() == CE_KEY_TAB)
                CE_TRACE("tab key is pressed (event)");
            CE_TRACE("{0}", (char)e.getKeyCode());
        }
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