#ifndef APPLICATION_H
#define APPLICATION_H

#include <minwindef.h>
#include <d3d11.h>
#include "Window.h"
#include "Event.h"
#include "Layer.h"
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
		static void SetWindow(Window* window);

		void CreateLayers();
		void CreateDirectx();
		static float GetDeltaTime();

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		static Application* privGetInstance();	

		static Application* instance;
		Window* pWindow = nullptr;
		HINSTANCE windowInstance;

		float deltaTime;


	};
}

#endif
