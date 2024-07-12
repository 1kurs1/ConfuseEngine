#include "cepch.h"
#include "Confuse/Renderer/RendererAPI.h"

#include <glad/glad.h>

namespace Confuse {
	void RendererAPI::init() {
		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	void RendererAPI::shutdown(){}
	
	void RendererAPI::clear(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::setClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void RendererAPI::drawIndexed(unsigned int count) {
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
}