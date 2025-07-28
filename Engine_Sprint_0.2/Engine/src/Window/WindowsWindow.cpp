#include "WindowsWindow.h"

#include "StateDirectXMan.h"
#include "StateRenderTargetView.h"

#include "ImGuizmo.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Colors.h"
#include "GameObjectManager.h"
#include "CameraNodeManager.h"

#include <d3d11.h>
#include <d3dcompiler.h>


namespace Azul
{
	LPCSTR WindowName = "EngineWindowClass";

	bool WindowsWindow::Create()
	{

		if (!CreateWindowClass() || !CreateWindowInstance())
		{
			assert(false && "Failed to create Win64 window.");
			return false;
		}

#ifdef _DEBUG
		WindowColor = Azul::Colors::LightBlue;
#else
		WindowColor = Azul::Colors::Wheat;
#endif

		Show();
		UpdateWindow(hwnd);

		return true;
	}

	void WindowsWindow::Destroy()
	{
		if (hwnd)
		{
			DestroyWindow(hwnd);
			hwnd = nullptr;
		}

		if (m_Data.Title)
		{
			delete[] m_Data.Title;
			m_Data.Title = nullptr;
		}

		UnregisterClass(TEXT("AzulWindowClass"), instance);
	}

	void WindowsWindow::Show()
	{
		ShowWindow(hwnd, SW_SHOW);
	}

	void WindowsWindow::Hide()
	{
		ShowWindow(hwnd, SW_HIDE);
	}

	void WindowsWindow::GUIClearnUp()
	{
		//GUI Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void WindowsWindow::SetupImGUI()
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

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(GetNativeHandle());
		ImGui_ImplDX11_Init(StateDirectXMan::GetDevice(), StateDirectXMan::GetContext());
	}

	void WindowsWindow::SetTitle(const char* title)
	{
		if (hwnd)
		{
			SetWindowTextA(hwnd, title);
		}

		if (m_Data.Title)
		{
			delete[] m_Data.Title;
			m_Data.Title = nullptr;
		}

		size_t len = strlen(title) + 1;
		m_Data.Title = new char[len];
		strcpy_s(m_Data.Title, len, title);
	}

	void WindowsWindow::OnUpdate(bool& quit)
	{
		MSG msg = { 0 };

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				quit = true;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowsWindow::DrawUI()
	{
		ImGuiIO& io = ImGui::GetIO();

		//ImGUI
		bool show_demo_window = false;
		bool show_another_window = false;

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&WindowColor); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		//GameObjects
		{
			static GameObjectNode* selectedObj = nullptr;

			PCSTree* activeTree = GameObjectManager::GetActiveGameObjects();

			PCSTreeForwardIterator itr(activeTree->GetRoot());

			ImGui::Begin("GameObject List");

			if (ImGui::BeginListBox("##GameObjectList", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
			{

				for (itr.First(); !itr.IsDone(); itr.Next())
				{
					GameObjectNode* obj = (GameObjectNode*)itr.Current();

					char name[64] = {};

					if (obj->GetName(name, sizeof(name)) != PCSNode::Code::SUCCESS)
					{
						strcpy_s(name, sizeof(name), "Unnamed Object");
					}

					if (ImGui::Selectable(name, obj == selectedObj))
					{
						selectedObj = obj;
					}

				}

				ImGui::EndListBox();
			}

			ImGui::End();

			//Draw Positions
			if (selectedObj != nullptr)
			{
				ImGui::Begin("Selected GameObject Position");

				Vec3* pos = selectedObj->GetPos();
				
				Vec3 temp(pos->x(), pos->y(), pos->z());


				if (ImGui::InputFloat3("Position", (float*)&temp, "%.3f"))
				{
					pos->x(temp.x());
					pos->y(temp.y());
					pos->z(temp.z());
				}

				Vec3* Scale = selectedObj->GetScale();
				Vec3 tempScale(*Scale);

				if (ImGui::InputFloat3("Scale", (float*)&tempScale, "%.3f"))
				{
					Scale->x(tempScale.x());
					Scale->y(tempScale.y());
					Scale->z(tempScale.z());
				}

				ImGui::End();

				/*	Gizmos Create Failed

				//Gizmos
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());

				//float windowWidth = (float)ImGui::GetWindowWidth();
				//float windowHeight = (float)ImGui::GetWindowHeight();

				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

				Camera* camera = CameraNodeManager::GetMainCam();

				Mat4 cameraView = camera->getViewMatrix().getT();
				Mat4 projection = camera->getProjMatrix().getT();
				Mat4 transForm = selectedObj->GetWorld()->getT();

				//Draw Gizmo
				ImGuizmo::Manipulate(
					(float*)&cameraView,
					(float*)&projection,
					ImGuizmo::OPERATION::TRANSLATE,
					ImGuizmo::LOCAL,
					(float*)&transForm
				);

				*/
			}
		}



		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
	}

	void WindowsWindow::OnRenderUI(StateRenderTargetView& renderTargetView)
	{
		ImGuiIO& io = ImGui::GetIO();

		static bool g_SwapChainOccluded = false;
		static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

		if (g_SwapChainOccluded && StateDirectXMan::GetSwapChain()->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
		{
			Sleep(10);
			return;
		}

		if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
		{
			renderTargetView.ClearnupRenderTarget();

			StateDirectXMan::GetSwapChain()->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
			g_ResizeWidth = g_ResizeHeight = 0;

			renderTargetView.Initialize();
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
		//Gizmo
		ImGuizmo::BeginFrame();

		DrawUI();

		// Rendering
		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		g_SwapChainOccluded = false;
	}

	WindowsWindow::WindowsWindow(HINSTANCE hInstance, const WindowProps& props)
		: instance( hInstance)
	{
		this->Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		this->Destroy();
	}

	// Forward declare message handler from imgui_impl_win32.cpp
	LRESULT WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT paintStruct;
		HDC hDC;

		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;

		switch (uMsg)
		{
		case WM_PAINT:
		{
			hDC = BeginPaint(hwnd, &paintStruct);
			EndPaint(hwnd, &paintStruct);
		}
		break;

		case WM_SIZE:
		{
			if (wParam == SIZE_MINIMIZED)
				return 0;

			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			
			//Event Send - Size Change
			break;
		}

		case WM_CHAR:
		{
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				return 0;
			}
			break;
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;

		default:
			break;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	Vec4 WindowsWindow::GetWindowColor()
	{
		return WindowColor;
	}

	bool WindowsWindow::CreateWindowClass()
	{

		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = this->WindowProc;
		wndClass.hInstance = this->instance;
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wndClass.hIcon = nullptr;
		wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndClass.lpszMenuName = nullptr;
		wndClass.lpszClassName = WindowName;
		wndClass.hIconSm = nullptr;

		if (!RegisterClassEx(&wndClass))
		{
			DWORD err = GetLastError();

			std::wstring msg = L"RegisterClassEx failed. Error code: " + std::to_wstring(err);
			MessageBoxW(nullptr, msg.c_str(), L"Error", MB_OK | MB_ICONERROR);

			assert(false && "Failed to register window class.");

			return false;
		}

		return true;
	}

	bool WindowsWindow::CreateWindowInstance()
	{
		RECT windowRect = { 0, 0, (LONG)m_Data.Width, (LONG)m_Data.Height };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		hwnd = CreateWindowA(WindowName, this->m_Data.Title,
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			nullptr, nullptr, instance, nullptr);

		return hwnd != nullptr;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		this->m_Data.Height = props.Height;
		this->m_Data.Width = props.Width;
		this->m_Data.Title = props.Title;

		m_Data.VSync = true;
	}

}
