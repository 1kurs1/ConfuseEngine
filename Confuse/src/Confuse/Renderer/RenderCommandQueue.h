#pragma once

#include "cepch.h"

namespace Confuse {
	class CONFUSE_API RenderCommandQueue
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		RenderCommandQueue();
		~RenderCommandQueue();

		void* allocate(RenderCommandFn func, unsigned int size);
		void execute();

	private:
		unsigned char* m_commandBuffer;
		unsigned char* m_commandBufferPtr;
		unsigned int m_commandCount = 0;
	};
}
