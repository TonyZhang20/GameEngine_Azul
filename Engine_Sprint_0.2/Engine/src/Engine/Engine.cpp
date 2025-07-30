//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Event.h"
#include "Engine.h"
#include "AnimTimer.h"

#include "LayerManager.h"
#include "StateDirectXMan.h"
#include "Application.h"




namespace Azul
{
	Engine::Engine()
		: Layer("Engine Layer"),
		mStateRenderTargetView(),
		mDepthStencilView(),
		mDepthStencilBuffer(),
		mStateDepthStencil(),
		mBlendStateOff(),
		mBlendStateAlpha(),
		mStateRasterizerSolid(),
		mStateRasterizerWireframe(),
		mViewport()
	{
			
	}


	Engine::~Engine()
	{
		StateDirectXMan::Destroy();
	}


	// ------------------------------------------------------------
	//  Initialize the Direct Device with Swap Chain
	// ------------------------------------------------------------

	int Engine::InitDirectX(HINSTANCE hInstance,HWND hwnd, BOOL vSync)
	{
		// AZUL_UNUSED_VAR(hInstance);
		// A window handle must have been created already.

		//assert(g_WindowHandle != nullptr);
		//RECT clientRect;
		//GetClientRect(g_WindowHandle, &clientRect);
		assert(IsWindow(hwnd));

		RECT clientRect;
		//HWND hwnd = (HWND)this->pWindow->GetNativeHandle();

		GetClientRect(hwnd, &clientRect);

		// Compute the exact client dimensions. This will be used
		// to initialize the render targets for our swap chain.
		unsigned int clientWidth = clientRect.right - clientRect.left;
		unsigned int clientHeight = clientRect.bottom - clientRect.top;

		Trace::out("%d %d\n", clientWidth, clientHeight);
		
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


	void Engine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN_ONE(Engine::OnWindowResizeEvent));
	}

	bool Engine::OnWindowClose(WindowCloseEvent& e)
	{
		this->quit = true;
		return true;
	}

	bool Engine::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		UINT width, height;
		width = e.GetWidth();
		height = e.GetHeight();

		//UnBind all target
		this->mStateRenderTargetView.CleanupRenderTarget();
		this->mDepthStencilView.ClearDepthStencilView();
		this->mDepthStencilBuffer.ClearDepthStencilBuffer();

		this->mStateRenderTargetView.UnBindAllRenderTarget();

		StateDirectXMan::ResizeSwapChain(width, height);

		//ReBind
		this->mStateRenderTargetView.Initialize();
		this->mDepthStencilBuffer.Initialize(width, height);
		this->mDepthStencilView.Initialize(this->mDepthStencilBuffer);
		
		//OmSet
		this->mStateRenderTargetView.Activate(this->mDepthStencilView);

		this->mViewport.ResizeViewPort(width, height);

		return false;
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

	void Engine::OnUpdate(float UpdateTime)
	{
		EngineTime.Tic();

		Update(UpdateTime);
		
		ClearDepthStencilBuffer();
		Render();

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
	}

}


// --- End of File ---
