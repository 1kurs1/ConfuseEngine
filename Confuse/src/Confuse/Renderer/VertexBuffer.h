#pragma once

#include "Confuse/Renderer/Renderer.h"

namespace Confuse {
	class CONFUSE_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer(){}

		virtual void setData(void* buffer, unsigned int size, unsigned int offset = 0) = 0;
		virtual void bind() const = 0;

		virtual unsigned int getSize() const = 0;
		virtual RendererID getRendererID() const = 0;

		static VertexBuffer* create(unsigned int size = 0);
	};
}