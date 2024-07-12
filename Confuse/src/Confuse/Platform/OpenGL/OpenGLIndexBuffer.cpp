#include "cepch.h"
#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

namespace Confuse {
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int size) : m_rendererID(0), m_size(size) {
		CE_RENDER_S({ glGenBuffers(1, &self->m_rendererID); });
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		CE_RENDER_S({glDeleteBuffers(1, &self->m_rendererID);});
	}

	void OpenGLIndexBuffer::setData(void* buffer, unsigned int size, unsigned int offset) {
		CE_RENDER_S3(buffer, size, offset, {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_rendererID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
		});
	}

	void OpenGLIndexBuffer::bind() const {
		CE_RENDER_S({
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->m_rendererID);
		});
	}
}