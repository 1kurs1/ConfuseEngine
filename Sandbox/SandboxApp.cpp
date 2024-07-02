#include <ConfuseEngine.h>

class DefaultLayer : public Confuse::Layer {
public:
	DefaultLayer() : Layer("default"){}

	void onUpdate() override {
		CE_INFO("default layer::update");
	}

	void onEvent(Confuse::Event& event) override {
		CE_TRACE("{0}", event.toString());
	}
};

class Sandbox :public Confuse::Application {
public:
	Sandbox(){
		pushLayer(new DefaultLayer());
	}
	~Sandbox(){}
};

Confuse::Application* Confuse::createApplication() {
	CE_WARN("sandbox app was created");
	return new Sandbox();
}