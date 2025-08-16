#include "Application.h"
#include "AnimTimer.h"
#include "ImGuiLayer.h"
#include "WindowsWindow.h"
#include "LayerManager.h"
#include "Game.h"
#include "Input.h"
namespace Azul
{
	Application* Application::instance = nullptr;

	Application* Application::Create(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
	{

		AZUL_UNUSED_VAR(prevInstance);
		AZUL_UNUSED_VAR(cmdLine);
		AZUL_UNUSED_VAR(cmdShow);

		if (instance == nullptr)
		{
			Application::instance = new Application();
			Application::instance->windowInstance = hInstance;

			return Application::instance;
		}
		else
		{
			assert(false);
			return nullptr;
		}
	}

	Application::Application()
	{
		LayerManager::Create();
		imGuiLayer = new ImGuiLayer();
	}

	Application::~Application()
	{

	}

	void Application::SetWindow(Window* window)
	{
		Application* application = Application::privGetInstance();
		
		//Set Event Rework
		//window->SetEventCallback();
		
		application->pWindow = window;

		if (!application->pWindow->Create())
		{
			MessageBox(nullptr, TEXT("Failed to create applicaiton window."), TEXT("Error"), MB_OK);
			assert(false);
			return;
		}

		application->pWindow->SetEventCallback(BIND_EVENT_FN_ONE(Application::OnEvent));
	}

	//Add Layer
	void Application::CreateLayers()
	{
		imGuiLayer->SetOrder(100);

		LayerManager::Add(imGuiLayer);

		Game* gameLayer = new Game();
		gameLayer->SetOrder(0);

		LayerManager::Add(gameLayer, nullptr);

		CreateDirectx();
		imGuiLayer->OnAttach();
	}

	//Init DirectX
	void Application::CreateDirectx()
	{
		Application* instance = Application::privGetInstance();

		Game* gameLayer = (Game*)LayerManager::Find("Engine Layer");
		assert(gameLayer);

		//will create statedirectXman
		if (gameLayer->InitDirectX(
			instance->windowInstance, 
			(HWND)instance->pWindow->GetNativeHandle(), 
			instance->pWindow->GetVsync()) != 0)
		{
			MessageBox(nullptr, TEXT("Failed to create DirectX device and swap chain."), TEXT("Error"), MB_OK);
			assert(false);
		}
	}


	float Application::GetDeltaTime()
	{
		static DWORD previousTime = timeGetTime();
		static const float targetFramerate = 60.0f;
		static const float maxTimeStep = 1.0f / targetFramerate;

		DWORD currentTime = timeGetTime();

		float deltaTime = (currentTime - previousTime) / 1000.0f;
		previousTime = currentTime;

		deltaTime = std::min<float>(deltaTime, maxTimeStep);

		Application::privGetInstance()->deltaTime = deltaTime;

		return deltaTime;
	}

	bool& Application::GetQuit()
	{
		// TODO: insert return statement here
		return Application::privGetInstance()->quit;
	}


	void Application::Run()
	{
		Application* app = Application::privGetInstance();
		Game* gameLayer = (Game*)LayerManager::Find("Engine Layer");

		if (!gameLayer->LoadContent())
		{
			MessageBox(nullptr, TEXT("Failed to load content."), TEXT("Error"), MB_OK);
			return;
		}

		AnimTimer t;

		while (!quit)
		{
			Input::Update();

			//Window Application Event
			app->GetWindow()->OnUpdate(quit);

			app->GetDeltaTime();

			LayerManager::Update(deltaTime);

			app->imGuiLayer->Begin();
			
			LayerManager::RenderImGui();

			app->imGuiLayer->End();
		
			app->GetWindow()->Present();

			//Trace::out("%f\n", deltaTime);
		}

		pWindow->Destroy();


		LayerManager::Destroy();
	}

	void Application::OnEvent(Event& e)
	{

		if (e.Handled) return;

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN_ONE(Application::OnWindowClose));

		//e.Print();

		PCSTreeForwardIterator itr = LayerManager::GetForwardActiveIterator();
		for (itr.First(); !itr.IsDone(); itr.Next())
		{
			Layer* layer = (Layer*)itr.Current();
			layer->OnEvent(e);

			if (e.Handled)
			{
				break;
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		return false;
	}


	void Application::PushLayer(Layer* layer)
	{
		//push
		LayerManager::AddByOrder(layer);
		layer->OnAttach();
	}

	Window* Application::GetWindow()
	{
		// TODO: insert return statement here
		Application* ins = Application::privGetInstance();

		assert(ins);
		assert(ins->pWindow);

		return ins->pWindow;
	}

	HINSTANCE Application::GetWindowInstance()
	{
		Application* ins = Application::privGetInstance();
		assert(ins);
		return ins->windowInstance;
	}


	Application* Application::privGetInstance()
	{
		return instance;
	}
}

