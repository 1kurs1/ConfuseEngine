#include <ConfuseEngine.h>

class Sandbox :public Confuse::Application {
public:
	Sandbox(){}
	~Sandbox(){}
};

Confuse::Application* Confuse::createApplication() {
	CE_WARN("sandbox app was created");
	return new Sandbox();
}