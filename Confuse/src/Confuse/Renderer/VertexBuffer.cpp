#include "cepch.h"

#include "Confuse/Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Confuse {
	VertexBuffer* VertexBuffer::create(unsigned int size) {
		switch (RendererAPI::current()) {
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::OpenGL: return new OpenGLVertexBuffer(size);
		}

		return nullptr;
	}
}