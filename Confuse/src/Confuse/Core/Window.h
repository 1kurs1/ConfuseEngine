#pragma once

#include <functional>
#include <string>

#include "Confuse/Core/Base.h"
#include "Confuse/Core/Events/Event.h"

namespace Confuse {
	struct WindowProps {
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& t = "Confuse Engine", unsigned int w = 1280, unsigned int h = 720) : title(t), width(w), height(h){}
	};

	class CONFUSE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window(){}

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}