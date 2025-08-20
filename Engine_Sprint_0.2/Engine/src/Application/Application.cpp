#include "Application.h"
#include "AnimTimer.h"
#include "EditorLayer.h"
#include "WindowsWindow.h"
#include "LayerManager.h"
#include "Game.h"
#include "Input.h"
#include "StateDirectXMan.h"
#include "Renderer.h"
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
		CreateDirectx();
		
		this->editorLayer = new EditorLayer();
		editorLayer->SetOrder(100);

		Game* gameLayer = new Game();
		Renderer* renderLayer = new Renderer(50, "Render Layer");

		gameLayer->SetOrder(0);
		LayerManager::Add(editorLayer);
		LayerManager::Add(renderLayer);
		LayerManager::Add(gameLayer, nullptr);
	}

	//Init DirectX
	void Application::CreateDirectx()
	{
		Application* instance = Application::privGetInstance();
		//will create statedirectXman
		//if (gameLayer->InitDirectX(
		//	instance->windowInstance, 
		//	(HWND)instance->pWindow->GetNativeHandle(), 
		//	instance->pWindow->GetVsync()) != 0)
		//{
		//	MessageBox(nullptr, TEXT("Failed to create DirectX device and swap chain."), TEXT("Error"), MB_OK);
		//	assert(false);
		//}
		HWND hwnd = (HWND)instance->pWindow->GetNativeHandle();

		assert(IsWindow(hwnd));
		StateDirectXMan::Create(hwnd, instance->pWindow->GetVsync());
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

		//TODO: EveryOne Can LoadContent -> as Awake
		if (!gameLayer->LoadContent())
		{
			MessageBox(nullptr, TEXT("Failed to load content."), TEXT("Error"), MB_OK);
			return;
		}

		AnimTimer t;

		LayerManager::Awake();
		LayerManager::Start();

		while (!quit)
		{
			Input::Update();

			//Window Application Event
			app->GetWindow()->OnUpdate(quit);
			app->GetDeltaTime();

			LayerManager::Update(app->deltaTime);
			
			//TODO: Render Óë Âß¼­ ½âñî
			LayerManager::Render(app->deltaTime);

			//swap chain
			app->GetWindow()->Present();
		}

		LayerManager::EndApplication();

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

