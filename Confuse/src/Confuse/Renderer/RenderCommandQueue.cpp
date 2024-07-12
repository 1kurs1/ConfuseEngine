#include "cepch.h"
#include "RenderCommandQueue.h"

#define CE_RENDER_TRACE(...) CE_CORE_TRACE(__VA_ARGS__)

namespace Confuse {
	RenderCommandQueue::RenderCommandQueue() {
		m_commandBuffer = new unsigned char[10 * 1024 * 1024];
		m_commandBufferPtr = m_commandBuffer;
		memset(m_commandBuffer, 0, 10 * 1024 * 1024);
	}

	RenderCommandQueue::~RenderCommandQueue()
	{
		delete[] m_commandBuffer;
	}

	void* RenderCommandQueue::allocate(const RenderCommandFn fn, unsigned int size)
	{
		*(RenderCommandFn*)m_commandBufferPtr = fn;
		m_commandBufferPtr += sizeof(RenderCommandFn);

		*(int*)m_commandBufferPtr = size;
		m_commandBufferPtr += sizeof(unsigned int);

		void* memory = m_commandBufferPtr;
		m_commandBufferPtr += size;
		m_commandCount++;
		return memory;
	}

	void RenderCommandQueue::execute()
	{
		CE_RENDER_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", m_commandCount, (m_commandBufferPtr - m_commandBuffer));

		byte* buffer = m_commandBuffer;

		for (unsigned int i = 0; i < m_commandCount; i++)
		{
			RenderCommandFn function = *(RenderCommandFn*)buffer;
			buffer += sizeof(RenderCommandFn);

			unsigned int size = *(unsigned int*)buffer;
			buffer += sizeof(unsigned int);
			function(buffer);
			buffer += size;
		}

		m_commandBufferPtr = m_commandBuffer;
		m_commandCount = 0;
	}
}