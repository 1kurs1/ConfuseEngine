#include <ConfuseEngine.h>

class Sandbox :public Confuse::Application {
public:
	Sandbox(){}
	~Sandbox(){}
};

Confuse::Application* Confuse::createApplication() {
	return new Sandbox();
}