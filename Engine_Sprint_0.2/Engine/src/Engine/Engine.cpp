//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Engine.h"
#include "AnimTimer.h"

#include "StateDirectXMan.h"



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

	Vec4 Engine::GetWindowColor()
	{
		return this->pWindow->GetWindowColor();
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

		this->pWindow->SetupImGUI();

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

		bool quit = false;


		AnimTimer EngineTime;

		if (!LoadContent())
		{
			MessageBox(nullptr, TEXT("Failed to load content."), TEXT("Error"), MB_OK);
			return -1;
		}


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

			this->pWindow->OnRenderUI(mStateRenderTargetView);
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

		UnloadContent();
		Cleanup();
		this->pWindow->GUIClearnUp();

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
