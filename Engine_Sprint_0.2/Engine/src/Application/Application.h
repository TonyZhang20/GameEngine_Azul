#ifndef APPLICATION_H
#define APPLICATION_H

#include <minwindef.h>
#include <d3d11.h>
#include "Window.h"
#include "Event.h"
#include "Layer.h"
#include "EditorLayer.h"
#include "ApplicationEvent.h"
#include "WindowsWindow.h"

namespace Azul
{
	class Application
	{
	public:
		Application();

		static Application* Create(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow);
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
	
		static Window* GetWindow();
		static HINSTANCE GetWindowInstance();
		inline static UINT GetWidth() { return Application::privGetInstance()->GetWindow()->GetWidth(); };
		inline static UINT GetHeight() { return Application::privGetInstance()->GetWindow()->GetHeight(); } ;
		void SetWindow(Window* window);

		void CreateLayers();
		void CreateDirectx();
		static float GetDeltaTime();
		static bool& GetQuit();
	public:
		bool quit = false;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		static Application* privGetInstance();	

		static Application* instance;
		bool showLoopTime = false;

		EditorLayer* editorLayer = nullptr;
		Window* pWindow = nullptr;

		HINSTANCE windowInstance;

		float deltaTime;


	};
}

#endif
