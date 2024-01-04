#include "CEpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Confuse{
    RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;
}