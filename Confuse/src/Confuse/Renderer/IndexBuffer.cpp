#include "cepch.h"

#include "Confuse/Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Confuse {
	IndexBuffer* IndexBuffer::create(unsigned int size) {
		switch (RendererAPI::current()) {
		case RendererAPIType::None : return nullptr;
		case RendererAPIType::OpenGL: return new OpenGLIndexBuffer(size);
		}
		return nullptr;
	}
}