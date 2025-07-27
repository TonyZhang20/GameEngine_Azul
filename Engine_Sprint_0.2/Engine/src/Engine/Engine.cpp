//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Engine.h"
#include "AnimTimer.h"

#include "StateDirectXMan.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace Azul
{
	Engine::Engine(const char* _pName, int _width, int _height)
		: 
		mStateRenderTargetView(),
		mDepthStencilView(),
		mDepthStencilBuffer(),
		mStateDepthStencil(),
		mBlendStateOff(),
		mBlendStateAlpha(),
		mStateRasterizerSolid(),
		mStateRasterizerWireframe(),
		mViewport(),
		pName(_pName),
		mWindowWidth(_width),
		mWindowHeight(_height)
	{
			
	}

	Engine::~Engine()
	{
		delete this->pWindow;
		StateDirectXMan::Destroy();
	}


	// ------------------------------------------------------------
	//  Initialize the Direct Device with Swap Chain
	// ------------------------------------------------------------

	int Engine::InitDirectX(HINSTANCE hInstance, BOOL vSync)
	{
		// AZUL_UNUSED_VAR(hInstance);
		// A window handle must have been created already.

		//assert(g_WindowHandle != nullptr);
		//RECT clientRect;
		//GetClientRect(g_WindowHandle, &clientRect);

		assert(this->pWindow != nullptr);
		RECT clientRect;
		HWND hwnd = (HWND)this->pWindow->GetNativeHandle();

		GetClientRect(hwnd, &clientRect);

		// Compute the exact client dimensions. This will be used
		// to initialize the render targets for our swap chain.
		unsigned int clientWidth = clientRect.right - clientRect.left;
		unsigned int clientHeight = clientRect.bottom - clientRect.top;
		StateDirectXMan::Create(hwnd, vSync);

		// The Direct3D device and swap chain were successfully created.
		// Now we need to initialize the buffers of the swap chain.
		// Next initialize the back buffer of the swap chain and associate it to a 
		// render target view.
		this->mStateRenderTargetView.Initialize();

		// Create the depth buffer for use with the depth/stencil view.
		this->mDepthStencilBuffer.Initialize(clientWidth, clientHeight);
		this->mDepthStencilView.Initialize(this->mDepthStencilBuffer);

		// Setup depth/stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{ 0 };

		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		// not using any stencil... its disabled
		depthStencilStateDesc.StencilEnable = FALSE;

		// Setup depth/stencil state.
		this->mStateDepthStencil.Initialize();

		// Setup blends state
		this->mBlendStateOff.Initialize(StateBlend::Mode::AlphaDisabled);

		// Disables Blending.... 
		this->mBlendStateAlpha.Initialize(StateBlend::Mode::AlphaEnabled);

		// Setup rasterizer state.
		this->mStateRasterizerSolid.Initialize(D3D11_FILL_SOLID, D3D11_CULL_FRONT);
		this->mStateRasterizerWireframe.Initialize(D3D11_FILL_WIREFRAME, D3D11_CULL_FRONT);

		// Initialize the viewport to occupy the entire client area.
		this->mViewport.Initialize(clientWidth, clientHeight);

		return 0;
	}

	// ------------------------------------
	// win main()
	// ------------------------------------
	int WINAPI Engine::wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
	{
		AZUL_UNUSED_VAR(prevInstance);
		AZUL_UNUSED_VAR(cmdLine);


		WindowProps props("Azul Engine", this->mWindowWidth, this->mWindowHeight);
		this->pWindow = new WindowsWindow(hInstance, props);

		if (!this->pWindow->Create())
		{
			MessageBox(nullptr, TEXT("Failed to create applicaiton window."), TEXT("Error"), MB_OK);
			return -1;
		}

		if (InitDirectX(hInstance, ENABLE_VSYNC) != 0)
		{
			MessageBox(nullptr, TEXT("Failed to create DirectX device and swap chain."), TEXT("Error"), MB_OK);
			return -1;
		}

		//Loop
		int returnCode = Run();

		return returnCode;
	}

	// ------------------------------------------------------
	// Find Refresh rate:
	// http://www.rastertek.com/dx11tut03.html
	// ------------------------------------------------------
	DXGI_RATIONAL Engine::QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync)
	{
		DXGI_RATIONAL refreshRate = { 0, 1 };
		if (vsync)
		{
			IDXGIFactory* factory = nullptr;
			IDXGIAdapter* adapter;
			IDXGIOutput* adapterOutput;
			DXGI_MODE_DESC* displayModeList;

			// Create a DirectX graphics interface factory.
			HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Could not create DXGIFactory instance."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to create DXGIFactory.");
			}

			hr = factory->EnumAdapters(0, &adapter);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to enumerate adapters."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to enumerate adapters.");
			}

			hr = adapter->EnumOutputs(0, &adapterOutput);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to enumerate adapter outputs."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to enumerate adapter outputs.");
			}

			UINT numDisplayModes;
			hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to query display mode list."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to query display mode list.");
			}

			displayModeList = new DXGI_MODE_DESC[numDisplayModes];
			assert(displayModeList);

			hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to query display mode list."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to query display mode list.");
			}

			// Now store the refresh rate of the monitor that matches the width and height of the requested screen.
			for (UINT i = 0; i < numDisplayModes; ++i)
			{
				if (displayModeList[i].Width == screenWidth && displayModeList[i].Height == screenHeight)
				{
					refreshRate = displayModeList[i].RefreshRate;
				}
			}

			delete[] displayModeList;
			SafeRelease(adapterOutput);
			SafeRelease(adapter);
			SafeRelease(factory);
		}

		return refreshRate;
	}

	// ------------------------------------
	// Run - main loop
	// ------------------------------------
	int Engine::Run()
	{
		static DWORD previousTime = timeGetTime();
		static const float targetFramerate = 30.0f;
		static const float maxTimeStep = 1.0f / targetFramerate;

		//ImGUI
		bool show_demo_window = false;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		bool quit = false;


		AnimTimer EngineTime;

		if (!LoadContent())
		{
			MessageBox(nullptr, TEXT("Failed to load content."), TEXT("Error"), MB_OK);
			return -1;
		}
#pragma region Setup GUI
		//SetUp ImGUI
		// Make process DPI aware and obtain main monitor scale
		ImGui_ImplWin32_EnableDpiAwareness();
		float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
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
		ImGui_ImplWin32_Init(pWindow->GetNativeHandle());
		ImGui_ImplDX11_Init(StateDirectXMan::GetDevice(), StateDirectXMan::GetContext());
#pragma endregion

		while (!quit)
		{
			EngineTime.Tic();

			//Windows Message
			this->pWindow->OnUpdate(quit);

			DWORD currentTime = timeGetTime();

			float deltaTime = (currentTime - previousTime) / 1000.0f;
			previousTime = currentTime;

			// Cap the delta time to the max time step (useful if your 
			// debugging and you don't want the deltaTime value to explode.
			deltaTime = std::min<float>(deltaTime, maxTimeStep);

			Update(deltaTime);

			ClearDepthStencilBuffer();

			Render();

#pragma region ImGui Loop

			static bool g_SwapChainOccluded = false;
			static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

			if (g_SwapChainOccluded && StateDirectXMan::GetSwapChain()->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
			{
				Sleep(10);
				continue;
			}

			if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
			{
				mStateRenderTargetView.ClearnupRenderTarget();

				StateDirectXMan::GetSwapChain()->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
				g_ResizeWidth = g_ResizeHeight = 0;

				mStateRenderTargetView.Initialize();
			}

			// Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			//I dont have demo here
			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			//if (show_demo_window)
				//ImGui::ShowDemoWindow(&show_demo_window);

			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
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

#pragma endregion


			//--------------------------------
			// Fast monitor sync
			//--------------------------------
			{
				AnimTime EngineLoopTime = EngineTime.Toc();

				// Current number of Frames... snap to the next 60Hz frame as target
				int NumFrames_60Hz = AnimTime::Quotient(EngineLoopTime, AnimTime(AnimTime::Duration::NTSC_FRAME));
				int TargetNumFrames_60Hz = NumFrames_60Hz + 1;
				AnimTime Target_60Hz = TargetNumFrames_60Hz * AnimTime(AnimTime::Duration::NTSC_FRAME);

				// we are before the flip... give a little cushion 
				AnimTime Overhead_Time = 500 * AnimTime(AnimTime::Duration::ONE_MICROSECOND);
				AnimTime EndTime = Target_60Hz - Overhead_Time;

				// Sit and spin.
				while (EngineLoopTime < EndTime)
				{
					EngineLoopTime = EngineTime.Toc();
				}
			}

			//--------------------------------------------------------
			// Wait for Vsync - flip front/back buffers
			//--------------------------------------------------------
			Present(ENABLE_VSYNC);
	
		}

		//GUI Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		UnloadContent();
		Cleanup();

		return 0;
	}

	// ------------------------------------
	// present - flip
	// ------------------------------------
	void Engine::Present(bool vSync)
	{
		IDXGISwapChain* swapChain = StateDirectXMan::GetSwapChain();
		if (vSync)
		{
			swapChain->Present(1, 0);
		}
		else
		{
			swapChain->Present(0, 0);
		}
	}

	// ------------------------------------
	// Cleanup()
	// ------------------------------------
	void Engine::Cleanup()
	{
		//SafeRelease(g_d3dSwapChain);
		//SafeRelease(g_d3dDeviceContext);

#ifdef _DEBUG
		HRESULT hr = S_OK;

		// Now we set up the Debug interface, to be queried on shutdown
		ID3D11Debug* debugDev = nullptr;
		hr = StateDirectXMan::GetDevice()->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));

		//debugDev->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL);
		//debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		//debugDev->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
		SafeRelease(debugDev);
#endif
		//ShutDown

		//SafeRelease(g_d3dDevice);
	}
}


// --- End of File ---
