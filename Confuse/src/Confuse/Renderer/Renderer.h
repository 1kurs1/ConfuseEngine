#pragma once

#include "RenderCommandQueue.h"
#include "RendererAPI.h"

namespace Confuse {
	class CONFUSE_API Renderer {
	public:
		typedef void(*RenderCommandFn)(void*);

		static void clear();
		static void clear(float r, float g, float b, float a = 1.0f);
		static void setClearColor(float r, float g, float b, float a);

		static void drawIndexed(unsigned int count);

		static void clearMagenta();

		static void init();

		static void* submit(RenderCommandFn fn, unsigned int size) {
			return s_instance->m_commandQueue.allocate(fn, size);
		}

		void waitAndRender();

		inline static Renderer& get() { return *s_instance; }

	private:
		static Renderer* s_instance;
		RenderCommandQueue m_commandQueue;
	};
}

#define CE_RENDER_PASTE2(a, b) a ## b
#define CE_RENDER_PASTE(a, b) CE_RENDER_PASTE2(a, b)
#define CE_RENDER_UNIQUE(x) CE_RENDER_PASTE(x, __LINE__)

#define CE_RENDER(code) \
    struct CE_RENDER_UNIQUE(CERenderCommand) \
    {\
        static void execute(void*)\
        {\
            code\
        }\
    };\
	{\
		auto mem = ::Confuse::Renderer::submit(CE_RENDER_UNIQUE(CERenderCommand)::execute, sizeof(CE_RENDER_UNIQUE(CERenderCommand)));\
		new (mem) CE_RENDER_UNIQUE(CERenderCommand)();\
	}\

#define CE_RENDER_1(arg0, code) \
	do{\
    struct CE_RENDER_UNIQUE(CERenderCommand) \
    {\
		CE_RENDER_UNIQUE(CERenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0) \
		: arg0(arg0) {}\
		\
        static void execute(void* argBuffer)\
        {\
			auto& arg0 = ((CE_RENDER_UNIQUE(CERenderCommand)*)argBuffer)->arg0;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    };\
	{\
		auto mem = ::Confuse::Renderer::submit(CE_RENDER_UNIQUE(CERenderCommand)::execute, sizeof(CE_RENDER_UNIQUE(CERenderCommand)));\
		new (mem) CE_RENDER_UNIQUE(CERenderCommand)(arg0);\
	}}while(0)

#define CE_RENDER_2(arg0, arg1, code) \
    struct CE_RENDER_UNIQUE(CERenderCommand) \
    {\
		CE_RENDER_UNIQUE(CERenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
		: arg0(arg0), arg1(arg1) {}\
		\
        static void execute(void* argBuffer)\
        {\
			auto& arg0 = ((CE_RENDER_UNIQUE(CERenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((CE_RENDER_UNIQUE(CERenderCommand)*)argBuffer)->arg1;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    };\
	{\
		auto mem = ::Confuse::Renderer::submit(CE_RENDER_UNIQUE(CERenderCommand)::execute, sizeof(CE_RENDER_UNIQUE(CERenderCommand)));\
		new (mem) CE_RENDER_UNIQUE(CERenderCommand)(arg0, arg1);\
	}\

#define CE_RENDER_3(arg0, arg1, arg2, code) \
    struct CE_RENDER_UNIQUE(CERenderCommand) \
    {\
		CE_RENDER_UNIQUE(CERenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
		: arg0(arg0), arg1(arg1), arg2(arg2) {}\
		\
        static void execute(void* argBuffer)\
        {\
			auto& arg0 = ((CE_RENDER_UNIQUE(CERenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((CE_RENDER_UNIQUE(CERenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((CE_RENDER_UNIQUE(CERenderCommand)*)argBuffer)->arg2;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
    };\
	{\
		auto mem = ::Confuse::Renderer::submit(CE_RENDER_UNIQUE(CERenderCommand)::execute, sizeof(CE_RENDER_UNIQUE(CERenderCommand)));\
		new (mem) CE_RENDER_UNIQUE(CERenderCommand)(arg0, arg1, arg2);\
	}\

#define CE_RENDER_4(arg0, arg1, arg2, arg3, code) \
    struct CE_RENDER_UNIQUE(CERenderCommand) \
    {\
		CE_RENDER_UNIQUE(CERenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
		\
        static void execute(void* argBuffer)\
        {\
			auto& arg0 = ((CE_RENDER_UNIQUE(CERenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((CE_RENDER_UNIQUE(CERenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((CE_RENDER_UNIQUE(CERenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((CE_RENDER_UNIQUE(CERenderCommand)*)argBuffer)->arg3;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
    };\
	{\
		auto mem = Renderer::submit(CE_RENDER_UNIQUE(CERenderCommand)::execute, sizeof(CE_RENDER_UNIQUE(CERenderCommand)));\
		new (mem) CE_RENDER_UNIQUE(CERenderCommand)(arg0, arg1, arg2, arg3);\
	}

#define CE_RENDER_S(code) auto self = this;\
	CE_RENDER_1(self, code)
#define CE_RENDER_S1(arg0, code) auto self = this;\
	CE_RENDER_2(self, arg0, code)
#define CE_RENDER_S2(arg0, arg1, code) auto self = this;\
	CE_RENDER_3(self, arg0, arg1, code)
#define CE_RENDER_S3(arg0, arg1, arg2, code) auto self = this;\
	CE_RENDER_4(self, arg0, arg1, arg2, code)