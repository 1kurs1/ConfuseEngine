#include "CEpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Confuse{
    VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size){
        switch(Renderer::getAPI()){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
        }

        CE_ASSERT(false, "unknown RenderAPI!");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t size){
        switch(Renderer::getAPI()){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size);
        }

        CE_ASSERT(false, "unknown RenderAPI!");
        return nullptr;
    }
}