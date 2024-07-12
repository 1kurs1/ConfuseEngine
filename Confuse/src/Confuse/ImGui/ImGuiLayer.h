#pragma once

#include "cepch.h"
#include "Confuse/Core/Layer.h"

namespace Confuse {
	class CONFUSE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(const std::string& name);
		virtual ~ImGuiLayer();

		void begin();
		void end();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;

	private:
		float m_time = 0.0f;
	};
}