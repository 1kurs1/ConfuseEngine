#pragma once

#include "Confuse/Core/Input.h"

namespace Confuse {
	class WindowsInput : public Input
	{
	protected:
		virtual bool isKeyPressedImpl(int keycode);
		virtual bool isMouseButtonPressedImpl(int button);
		virtual float getMouseXImpl();
		virtual float getMouseYImpl();
	};
}