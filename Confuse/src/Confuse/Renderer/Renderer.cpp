#include "cepch.h"
#include "Renderer.h"

namespace Confuse {
	Renderer* Renderer::s_instance = new Renderer();
	RendererAPIType RendererAPI::s_currentRendererAPI = RendererAPIType::OpenGL;

	void Renderer::init(){
		CE_RENDER({ RendererAPI::init(); });
	}
	
	void Renderer::clear(){}
	
	void Renderer::clear(float r, float g, float b, float a) {
		CE_RENDER_4(r, g, b, a, {
			RendererAPI::clear(r, g, b, a);
		});
	}

	void Renderer::clearMagenta() {
		clear(1, 0, 1);
	}

	void Renderer::setClearColor(float r, float g, float b, float a){}

	void Renderer::drawIndexed(unsigned int count) {
		CE_RENDER_1(count, {
			RendererAPI::drawIndexed(count);
		});
	}

	void Renderer::waitAndRender() {
		s_instance->m_commandQueue.execute();
	}
}