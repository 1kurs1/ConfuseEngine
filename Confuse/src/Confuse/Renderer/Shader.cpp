#include "CEpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Confuse{
    Shader* Shader::create(const std::string& vertexSrc, const std::string& fragmentSrc){
        switch(Renderer::getAPI()){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return new OpenGLShader(vertexSrc, fragmentSrc);
        }

        CE_ASSERT(false, "unknown RenderAPI!");
        return nullptr;
    }
}