#pragma once

#include "Confuse/Renderer/VertexBuffer.h"

namespace Confuse {
	class CONFUSE_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(unsigned int size);
		virtual ~OpenGLVertexBuffer();

		virtual void setData(void* buffer, unsigned int size, unsigned int offset = 0);
		virtual void bind() const;

		virtual unsigned int getSize() const { return m_size; }
		virtual RendererID getRendererID() const { return m_rendererID; }

	private:
		RendererID m_rendererID;
		unsigned int m_size;
	};
}