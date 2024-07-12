#pragma once

#include "Confuse/Core/Base.h"

namespace Confuse {
	enum class CONFUSE_API TextureFormat {
		None = 0,
		RGB = 1,
		RGBA = 2,
	};

	class CONFUSE_API Texture {
	public:
		virtual ~Texture(){}
	};

	class CONFUSE_API Texture2D : public Texture
	{
	public:
		static Texture2D* create(TextureFormat format, unsigned int width, unsigned int height);

		virtual TextureFormat getFormat() const = 0;
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
	};
}