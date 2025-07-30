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

		ImGuiLayer& operator = (ImGuiLayer&) = delete;
		ImGuiLayer(ImGuiLayer&) = delete;

		static ImGuiKey MapVirtualKeyToImGuiKey(int key);

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMoveEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnWindowResizedEvent(WindowResizeEvent& e);

	private:
	};
}

#endif // DEBUG
