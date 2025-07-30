#include "ImGuiLayer.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "StateDirectXMan.h"
#include "Application.h"

namespace Azul
{
	ImGuiLayer::ImGuiLayer() 
		: Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnAttach()
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
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate(float UpdateTime)
	{
		ImGuiIO& io = ImGui::GetIO();

		static bool g_SwapChainOccluded = false;
		static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		io.DisplaySize = ImVec2(Application::GetWindow()->GetWidth(), Application::GetWindow()->GetHeight());
		io.DeltaTime = UpdateTime;
		
		static bool showWindow = true;
		ImGui::ShowDemoWindow(&showWindow);

		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}



	void ImGuiLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN_ONE(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN_ONE(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN_ONE(ImGuiLayer::OnMouseMoveEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN_ONE(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN_ONE(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN_ONE(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN_ONE(ImGuiLayer::OnWindowResizedEvent));

	}

	static ImGuiKey MapVirtualKeyToImGuiKey(int key)
	{
		switch (key)
		{
			case VK_TAB: return ImGuiKey_Tab;
			case VK_LEFT: return ImGuiKey_LeftArrow;
			case VK_RIGHT: return ImGuiKey_RightArrow;
			case VK_UP: return ImGuiKey_UpArrow;
			case VK_DOWN: return ImGuiKey_DownArrow;
			case VK_PRIOR: return ImGuiKey_PageUp;
			case VK_NEXT: return ImGuiKey_PageDown;
			case VK_HOME: return ImGuiKey_Home;
			case VK_END: return ImGuiKey_End;
			case VK_INSERT: return ImGuiKey_Insert;
			case VK_DELETE: return ImGuiKey_Delete;
			case VK_BACK: return ImGuiKey_Backspace;
			case VK_SPACE: return ImGuiKey_Space;
			case VK_RETURN: return ImGuiKey_Enter;
			case VK_ESCAPE: return ImGuiKey_Escape;
			case VK_CONTROL: return ImGuiKey_LeftCtrl;
			case VK_MENU: return ImGuiKey_LeftAlt;
			case VK_SHIFT: return ImGuiKey_LeftShift;
			case VK_LWIN: return ImGuiKey_LeftSuper;
			case VK_RWIN: return ImGuiKey_RightSuper;
			case VK_APPS: return ImGuiKey_Menu;

			case 'A': return ImGuiKey_A;
			case 'B': return ImGuiKey_B;
			case 'C': return ImGuiKey_C;
			case 'D': return ImGuiKey_D;
			case 'E': return ImGuiKey_E;
			case 'F': return ImGuiKey_F;
			case 'G': return ImGuiKey_G;
			case 'H': return ImGuiKey_H;
			case 'I': return ImGuiKey_I;
			case 'J': return ImGuiKey_J;
			case 'K': return ImGuiKey_K;
			case 'L': return ImGuiKey_L;
			case 'M': return ImGuiKey_M;
			case 'N': return ImGuiKey_N;
			case 'O': return ImGuiKey_O;
			case 'P': return ImGuiKey_P;
			case 'Q': return ImGuiKey_Q;
			case 'R': return ImGuiKey_R;
			case 'S': return ImGuiKey_S;
			case 'T': return ImGuiKey_T;
			case 'U': return ImGuiKey_U;
			case 'V': return ImGuiKey_V;
			case 'W': return ImGuiKey_W;
			case 'X': return ImGuiKey_X;
			case 'Y': return ImGuiKey_Y;
			case 'Z': return ImGuiKey_Z;

			case '0': return ImGuiKey_0;
			case '1': return ImGuiKey_1;
			case '2': return ImGuiKey_2;
			case '3': return ImGuiKey_3;
			case '4': return ImGuiKey_4;
			case '5': return ImGuiKey_5;
			case '6': return ImGuiKey_6;
			case '7': return ImGuiKey_7;
			case '8': return ImGuiKey_8;
			case '9': return ImGuiKey_9;

			case VK_F1: return ImGuiKey_F1;
			case VK_F2: return ImGuiKey_F2;
			case VK_F3: return ImGuiKey_F3;
			case VK_F4: return ImGuiKey_F4;
			case VK_F5: return ImGuiKey_F5;
			case VK_F6: return ImGuiKey_F6;
			case VK_F7: return ImGuiKey_F7;
			case VK_F8: return ImGuiKey_F8;
			case VK_F9: return ImGuiKey_F9;
			case VK_F10: return ImGuiKey_F10;
			case VK_F11: return ImGuiKey_F11;
			case VK_F12: return ImGuiKey_F12;

			default: return ImGuiKey_None;
		}
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnMouseMoveEvent(MouseMovedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& e)
	{
		return false;
	}

}
