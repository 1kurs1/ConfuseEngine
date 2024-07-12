#pragma once

#include "Confuse/Renderer/RendererAPI.h"
#include "Confuse/Renderer/Texture.h"

namespace Confuse {
	class CONFUSE_API OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(TextureFormat format, unsigned int width, unsigned int height);
		~OpenGLTexture2D();

		virtual TextureFormat getFormat() const { return m_format; }
		virtual unsigned int getWidth() const { return m_width; }
		virtual unsigned int getHeight() const { return m_height; }

	private:
		RendererID m_rendererID;
		TextureFormat m_format;
		unsigned int m_width, m_height;
	};
}