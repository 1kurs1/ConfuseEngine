#pragma once

namespace Confuse {

	using RendererID = unsigned int;

	enum class CONFUSE_API RendererAPIType {
		None,
		OpenGL
	};

	class CONFUSE_API RendererAPI {
	public:
		static void init();
		static void shutdown();

		static void clear(float r, float g, float b, float a);
		static void setClearColor(float r, float g, float b, float a);

		static void drawIndexed(unsigned int count);

		static RendererAPIType current() { return s_currentRendererAPI; }

	private:
		static RendererAPIType s_currentRendererAPI;
	};
}