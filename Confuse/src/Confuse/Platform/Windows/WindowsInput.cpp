#include "cepch.h"
#include "WindowsInput.h"
#include "WindowsWindow.h"

#include "Confuse/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Confuse {
	bool WindowsInput::isKeyPressedImpl(int keycode) {
		auto& window = static_cast<WindowsWindow&>(Application::get().getWindow());
		auto state = glfwGetKey(static_cast<GLFWwindow*>(window.getNativeWindow()), keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::isMouseButtonPressedImpl(int button) {
		auto& window = static_cast<WindowsWindow&>(Application::get().getWindow());
		auto state = glfwGetKey(static_cast<GLFWwindow*>(window.getNativeWindow()), button);
		return state == GLFW_PRESS;
	}

	float WindowsInput::getMouseXImpl() {
		auto& window = static_cast<WindowsWindow&>(Application::get().getWindow());

		double xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.getNativeWindow()), &xpos, &ypos);

		return (float)xpos;
	}
	
	float WindowsInput::getMouseYImpl() {
		auto& window = static_cast<WindowsWindow&>(Application::get().getWindow());

		double xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.getNativeWindow()), &xpos, &ypos);

		return (float)ypos;
	}
}