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

#include "ApplicationEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

#include "LayerManager.h"
#include "Layer.h"


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

		return true;
	}

	void WindowsWindow::Destroy()
	{
		if (m_hwnd)
		{
			DestroyWindow(m_hwnd);
			m_hwnd = nullptr;
		}

		if (m_Data.Title)
		{
			delete[] m_Data.Title;
			m_Data.Title = nullptr;
		}

		UnregisterClass(WindowName, instance);

		delete this;
	}

	void WindowsWindow::Show()
	{
		ShowWindow(m_hwnd, SW_SHOW);
	}

	void WindowsWindow::Hide()
	{
		ShowWindow(m_hwnd, SW_HIDE);
	}

	void WindowsWindow::SetVsync(bool b)
	{
		this->m_Data.VSync = b;
	}

	void WindowsWindow::Present()
	{
		IDXGISwapChain* swapChain = StateDirectXMan::GetSwapChain();

		if (m_Data.VSync)
		{
			swapChain->Present(1, 0);
		}
		else
		{
			swapChain->Present(0, 0);
		}
	}

	void WindowsWindow::SetTitle(const char* title)
	{
		if (m_hwnd)
		{
			SetWindowTextA(m_hwnd, title);
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
			if (msg.wParam == VK_ESCAPE)
			{
				quit = true;
			}

			if (msg.message == WM_QUIT)
			{
				quit = true;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowsWindow::OnEvent(Event& e)
	{

	}

	void WindowsWindow::OnWindowClose(Event& e)
	{

	}

	void WindowsWindow::QuitCallBack()
	{
		WindowCloseEvent e;
		this->m_Data.EventCallback(e);
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

			PCSTreeForwardIterator itr(GameObjectManager::GetActiveGameObjects()->GetRoot());

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

	WindowsWindow::WindowsWindow(HINSTANCE hInstance, const WindowProps& props)
		: instance( hInstance)
	{
		this->Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{

	}

	// Forward declare message handler from imgui_impl_win32.cpp
	LRESULT WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowsWindow* pThis = nullptr;


		if (uMsg == WM_NCCREATE)
		{
			// 从创建参数获取this指针
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = static_cast<WindowsWindow*>(pCreate->lpCreateParams);

			// 将this指针存储在窗口的额外数据中
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		}
		else 
		{
			// 从窗口额外数据获取this指针
			pThis = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		switch (uMsg)
		{
		case WM_CHAR:
		{
			if (wParam == VK_ESCAPE)
			{
				//QuitCallBack();
				PostQuitMessage(0);
				break;
			}
		}
		break;

		case WM_DESTROY:
		{
			//QuitCallBack();
			PostQuitMessage(0);
			break;
		}
		default:
			break;
		}


		if (pThis) 
		{
			return pThis->HandleMessage(hwnd, uMsg, wParam, lParam);
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	LRESULT WindowsWindow::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT paintStruct;
		HDC hDC;

		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
			return true;


		if (!m_Data.EventCallback)
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

		switch (uMsg)
		{
		case WM_PAINT:
		{
			hDC = BeginPaint(hwnd, &paintStruct);
			EndPaint(hwnd, &paintStruct);
		}
		break;

		case WM_CHAR:
		{
			if (wParam == VK_ESCAPE)
			{
				QuitCallBack();
				PostQuitMessage(0);
			}
			break;
		}
		break;

		case WM_DESTROY:
		{
			QuitCallBack();
			PostQuitMessage(0);
		}
		break;

		case WM_MOUSEMOVE:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			MouseMovedEvent e(pt.x, pt.y);
			m_Data.EventCallback(e);

			break;
		}
		case WM_LBUTTONDOWN:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);
			MouseButtonPressedEvent e(0, pt.x, pt.y);
			m_Data.EventCallback(e);
			break;
		}
		case WM_LBUTTONUP:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			MouseButtonReleasedEvent e(0, pt.x, pt.y);
			m_Data.EventCallback(e);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			MouseButtonPressedEvent e(1, pt.x, pt.y);
			m_Data.EventCallback(e);
			break;
		}
		case WM_RBUTTONUP:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			MouseButtonReleasedEvent e(1, pt.x, pt.y);
			m_Data.EventCallback(e);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			MouseButtonPressedEvent e(2, pt.x, pt.y);
			m_Data.EventCallback(e);
			break;
		}
		case WM_MBUTTONUP:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			MouseButtonReleasedEvent e(2, pt.x, pt.y);
			m_Data.EventCallback(e);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam); // 120 * n

			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			MouseScrolledEvent e(pt.x, pt.y, delta);
			m_Data.EventCallback(e);
			break;
		}

		case WM_SIZE:
		{
			if (wParam == SIZE_MINIMIZED)
				return 0;

			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);

			m_Data.Width = width;
			m_Data.Height = height;

			WindowResizeEvent resizeEvent(width, height);
			m_Data.EventCallback(resizeEvent);
		
			break;
		}
		case WM_KEYDOWN:
		{
			int keyCode = (int)wParam;

			KeyPressedEvent event(keyCode, 0);
			m_Data.EventCallback(event);

			if (keyCode == VK_ESCAPE)
			{
				QuitCallBack();
				PostQuitMessage(0);
			}
			break;
		}
		case WM_KEYUP:
		{
			int keyCode = (int)wParam;
			KeyReleasedEvent event(keyCode);
			m_Data.EventCallback(event);
			break;
		}

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
		wndClass.lpfnWndProc = &this->WindowProc;
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

		m_hwnd = CreateWindowA(
			WindowName, 
			this->m_Data.Title,
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			nullptr, nullptr, instance, this);

		if (!m_hwnd)
		{
			DWORD err = GetLastError();
			std::wstring msg = L"CreateWindowEx failed. Error code: " + std::to_wstring(err);
			MessageBoxW(nullptr, msg.c_str(), L"Window Creation Error", MB_OK | MB_ICONERROR);
		}

		return m_hwnd != nullptr;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		this->m_Data.Height = props.Height;
		this->m_Data.Width = props.Width;
		this->m_Data.Title = props.Title;

		m_Data.VSync = true;
	}

}
