#include "EditorLayer.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "StateDirectXMan.h"
#include "Application.h"
#include "ImGuiBuild.h"
#include "Components.h"
#include "ZEntity.h"
#include "Event.h"

namespace Azul
{
	EditorLayer::EditorLayer() 
		: Layer("EditorLayer"), pbufferFrame(nullptr)
	{
		buildWindow = new ImGuiBuild();
	}

	EditorLayer::~EditorLayer()
	{
		delete buildWindow;
		delete pbufferFrame;
		//delete activeScene;

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();

		ImGui::DestroyContext();
	}

	void EditorLayer::Awake()
	{
		pbufferFrame = new BufferFrame();
	}

	void EditorLayer::OnAttach()
	{
		//SetUp ImGUI
		// Make process DPI aware and obtain main monitor scale
		ImGui_ImplWin32_EnableDpiAwareness();
		float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup scaling
		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
		style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
		io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
		io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplWin32_Init(Application::GetWindow()->GetNativeHandle());
		ImGui_ImplDX11_Init(StateDirectXMan::GetDevice(), StateDirectXMan::GetContext());

		//activeScene = new Scene();

	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnRender(float deltaTim)
	{
		//Begin Frame
		Begin();

		static bool showWindow = true;
		//ImGui::ShowDemoWindow(&showWindow);

		buildWindow->DrawWindow(showWindow);

		//End Frame
		End();
	}

	void EditorLayer::OnUpdate(float UpdateTime)
	{


		//activeScene->Update(UpdateTime);
	}

	void EditorLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN_ONE(EditorLayer::OnWindowResize));
	}

	void EditorLayer::Begin()
	{
		//Copy Data
		this->pbufferFrame->SetActive();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void EditorLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();

		static bool g_SwapChainOccluded = false;
		static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

		io.DisplaySize = ImVec2(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());

		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void EditorLayer::OnImGuiRender()
	{

	}

	bool EditorLayer::OnWindowResize(WindowResizeEvent& e)
	{
		this->pbufferFrame->OnResize();

		return false;
	}

}
