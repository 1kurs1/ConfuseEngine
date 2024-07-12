#include "cepch.h"
#include <glad/glad.h>
#include <imgui.h>

#include "WindowsWindow.h"

#include "Confuse/Core/Events/ApplicationEvent.h"
#include "Confuse/Core/Events/KeyEvent.h"
#include "Confuse/Core/Events/MouseEvent.h"

namespace Confuse {
	static void GLFWErrorCallback(int error, const char* description) {
		CE_CORE_ERROR("glfw error ({0}): {1}", error, description);
	}

	static bool s_glfwInitialized = false;

	Window* Window::create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props){
		init(props);
	}
	WindowsWindow::~WindowsWindow(){}

	void WindowsWindow::init(const WindowProps& props) {
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		CE_CORE_INFO("creating window {0} ({1}, {2})", props.title, props.width, props.height);
	
		if (!s_glfwInitialized) {
			int success = glfwInit();
			CE_CORE_ASSERT(success, "could not initialize glfw!");
			glfwSetErrorCallback(GLFWErrorCallback);

			s_glfwInitialized = true;
		}

		m_window = glfwCreateWindow((int)props.width, (int)props.height, m_data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CE_CORE_ASSERT(status, "failed to initialize glad!");
		glfwSetWindowUserPointer(m_window, &m_data);
		setVSync(true);

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

			WindowResizeEvent event((unsigned int)width, (unsigned int)height);
			data.eventCallback(event);
			data.width = width;
			data.height = height;
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.eventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codeinput) {
			auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

			KeyTypedEvent event((int)codeinput);
			data.eventCallback(event);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

			switch (action) {
			case GLFW_PRESS: {
				MouseButtonPressedEvent event(button);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
			auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y) {
			auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

			MouseMovedEvent event((float)x, (float)y);
			data.eventCallback(event);
		});

		m_imguiMouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_imguiMouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		m_imguiMouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_imguiMouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
		m_imguiMouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		m_imguiMouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_imguiMouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_imguiMouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	}

	void WindowsWindow::shutdown(){}

	void WindowsWindow::onUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_window);

		ImGuiMouseCursor imguiCursor = ImGui::GetMouseCursor();
		glfwSetCursor(m_window, m_imguiMouseCursors[imguiCursor] ? m_imguiMouseCursors[imguiCursor] : m_imguiMouseCursors[ImGuiMouseCursor_Arrow]);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void WindowsWindow::setVSync(bool enabled) {
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);
		m_data.vSync = enabled;
	}

	bool WindowsWindow::isVSync() const {
		return m_data.vSync;
	}
}