#include "cepch.h"
#include "Application.h"

#include "Confuse/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Confuse {
	#define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;

	Application::Application(){
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(BIND_EVENT_FN(onEvent));

		m_imGuiLayer = new ImGuiLayer("imgui");
		pushOverlay(m_imGuiLayer);

		Renderer::init();
	}
	Application::~Application(){}

	void Application::pushLayer(Layer* layer) {
		m_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::renderImGui() {
		m_imGuiLayer->begin();
		for (Layer* layer : m_layerStack)
			layer->onImGuiRender();
		m_imGuiLayer->end();
	}

	void Application::pushOverlay(Layer* overlay) {
		m_layerStack.pushOverlay(overlay);
		overlay->onAttach();
	}

	void Application::run() {
		onInit();
		while (m_running) {
			for (Layer* layer : m_layerStack)
				layer->onUpdate();

			Application* app = this;
			CE_RENDER_1(app, { app->renderImGui(); });

			Renderer::get().waitAndRender();
			m_window->onUpdate();
		}
		onShutdown();
	}

	void Application::onEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
			(*--it)->onEvent(event);
			if (event.handled) break;
		}
	}

	bool Application::onWindowResize(WindowResizeEvent& e) {
		return false;
	}

	bool Application::onWindowClose(WindowCloseEvent& e) {
		m_running = false;
		return true;
	}
}