#pragma once

#include <GLFW/glfw3.h>

#include "Confuse/Core/Window.h"

namespace Confuse {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void onUpdate() override;


		inline unsigned int getWidth() const override { return m_data.width; }
		inline unsigned int getHeight() const override { return m_data.height; }

		inline void setEventCallback(const EventCallbackFn& callback) override { m_data.eventCallback = callback; }
		void setVSync(bool enabled);
		bool isVSync() const;

		inline void* getNativeWindow() const { return m_window; }

	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();

	private:
		GLFWwindow* m_window;
		GLFWcursor* m_imguiMouseCursors[9] = { 0 };

		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallback;
		};

		WindowData m_data;
	};
}