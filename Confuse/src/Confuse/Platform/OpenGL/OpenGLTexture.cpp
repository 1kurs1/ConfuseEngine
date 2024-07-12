#include "cepch.h"
#include "OpenGLTexture.h"

#include "Confuse/Renderer/RendererAPI.h"
#include "Confuse/Renderer/Renderer.h"

#include <glad/glad.h>

namespace Confuse {
	static GLenum ConfuseToOpenGLTextureFormat(TextureFormat format) {
		switch (format) {
		case Confuse::TextureFormat::RGB: return GL_RGB;
		case Confuse::TextureFormat::RGBA: return GL_RGBA;
		}
		return 0;
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, unsigned int width, unsigned int height) : m_rendererID(0), m_format(format), m_width(width), m_height(height) {
		auto self = this;
		CE_RENDER_1(self, {
			glGenTextures(1, &self->m_rendererID);
			glBindTexture(GL_TEXTURE_2D, self->m_rendererID);
			glTexImage2D(GL_TEXTURE_2D, 0, ConfuseToOpenGLTextureFormat(self->m_format), self->m_width, self->m_height, 0, ConfuseToOpenGLTextureFormat(self->m_format), GL_UNSIGNED_BYTE, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);
		});
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		auto self = this;
		CE_RENDER_1(self, {
			glDeleteTextures(1, &self->m_rendererID);
		});
	}
}