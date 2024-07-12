#include "cepch.h"
#include "Texture.h"

#include "Confuse/Renderer/RendererAPI.h"
#include "Confuse/Platform/OpenGL/OpenGLTexture.h"

namespace Confuse {
	Texture2D* Texture2D::create(TextureFormat format, unsigned int width, unsigned int height) {
		switch (RendererAPI::current()) {
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::OpenGL: return new OpenGLTexture2D(format, width, height);
		}
		return nullptr;
	}
}