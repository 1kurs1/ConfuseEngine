#include "cepch.h"
#include "OpenGLVertexBuffer.h"

#include <Glad/glad.h>

namespace Confuse {
	OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int size) :m_rendererID(0), m_size(size) {
		CE_RENDER_S({
			glGenBuffers(1, &self->m_rendererID);
		});
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		CE_RENDER_S({
			glDeleteBuffers(1, &self->m_rendererID);
		});
	}

	void OpenGLVertexBuffer::setData(void* buffer, unsigned int size, unsigned int offset) {
		CE_RENDER_S3(buffer, size, offset, {
			glBindBuffer(GL_ARRAY_BUFFER, self->m_rendererID);
			glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
		
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		});
	}

	void OpenGLVertexBuffer::bind() const {
		CE_RENDER_S({
			glBindBuffer(GL_ARRAY_BUFFER, self->m_rendererID);
		});
	}
}