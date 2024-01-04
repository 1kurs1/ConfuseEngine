#include "CEpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Confuse{
    VertexArray* VertexArray::create(){
        switch(Renderer::getAPI()){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
        }

        CE_ASSERT(false, "unknown RenderAPI!");
        return nullptr;
    }
}