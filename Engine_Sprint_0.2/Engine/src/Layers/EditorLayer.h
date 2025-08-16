#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Scene.h"
#include "imgui.h"
#include "Layer.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include "Entity.h"
#include "ApplicationEvent.h"


namespace Azul
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float UpdateTime) override;
		virtual void OnEvent(class Event& event) override;
		
		void Begin();
		void End();
		virtual void OnImGuiRender() override;

		EditorLayer& operator = (EditorLayer&) = delete;
		EditorLayer(EditorLayer&) = delete;

		template<typename T>
		inline ZVector<T> FindAllComponentsBy()
		{
			return this->activeScene->FindObjectsBy<T>();
		}


	private:
		class ImGuiBuild* buildWindow;
		Scene* activeScene;

	};
}

#endif // DEBUG
