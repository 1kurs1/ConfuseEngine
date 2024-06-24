#pragma once

#include "Core.h"

namespace Confuse {
	class CONFUSE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	Application* createApplication();
}