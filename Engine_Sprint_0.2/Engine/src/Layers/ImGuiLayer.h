#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Layer.h"

namespace Azul
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float UpdateTime) override;
		virtual void OnEvent(class Event& event) override;

		ImGuiLayer& operator = (ImGuiLayer&) = delete;
		ImGuiLayer(ImGuiLayer&) = delete;

	private:
	};
}

#endif // DEBUG
