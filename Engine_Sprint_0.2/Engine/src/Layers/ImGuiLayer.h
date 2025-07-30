#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "imgui.h"
#include "Layer.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include "ApplicationEvent.h"

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
		
		void Begin();
		void End();
		virtual void OnImGuiRender() override;

		ImGuiLayer& operator = (ImGuiLayer&) = delete;
		ImGuiLayer(ImGuiLayer&) = delete;

		static ImGuiKey MapVirtualKeyToImGuiKey(int key);

	private:


	private:
	};
}

#endif // DEBUG
