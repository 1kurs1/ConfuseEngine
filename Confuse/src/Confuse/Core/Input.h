#pragma once

namespace Confuse {
	class Input {
	public:
		static bool isKeyPressed(int keycode) { return s_instance->isKeyPressed(keycode); }

		inline static bool isMouseButtonPressed(int button) { return s_instance->isMouseButtonPressed(button); }
		inline static float getMouseX() { return s_instance->getMouseX(); }
		inline static float getMouseY() { return s_instance->getMouseY(); }

	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0;
		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;

	private:
		static Input* s_instance;
	};
}