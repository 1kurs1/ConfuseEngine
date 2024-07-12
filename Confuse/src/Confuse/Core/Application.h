#pragma once

#include "Confuse/Core/Base.h"
#include "Confuse/Core/Window.h"
#include "Confuse/Core/LayerStack.h"

#include "Confuse/Core/Events/ApplicationEvent.h"

#include "Confuse/ImGui/ImGuiLayer.h"

namespace Confuse {
	class CONFUSE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		virtual void onInit(){}
		virtual void onShutdown(){}
		virtual void onUpdate(){}

		virtual void onEvent(Event& event);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);
		void renderImGui();

		inline Window& getWindow() { return *m_window; }
		static inline Application& get() { return *s_instance; }

	private:
		bool onWindowResize(WindowResizeEvent& e);
		bool onWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
		LayerStack m_layerStack;
		ImGuiLayer* m_imGuiLayer;

		static Application* s_instance;
	};

	Application* createApplication();	// implementation on client app
}