//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "StateDirectXMan.h"
#include "Engine.h"
#include "StateAdapter.h"
#include "StateAdapterOutput.h"
#include "StateFactory.h"

namespace Azul
{
	StateDirectXMan *StateDirectXMan::posInstance = nullptr;

#if _DEBUG
#define createDeviceFlags_define  D3D11_CREATE_DEVICE_DEBUG

#else
#define createDeviceFlags_define  0
#endif


	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
	void StateDirectXMan::Create(HWND g_WindowHandle, bool vSync)
	{
		// A window handle must have been created already.
		assert(g_WindowHandle != nullptr);

		// intialize the singleton here
		assert(posInstance == nullptr);

		// Do the initialization
		if (posInstance == nullptr)
		{
			posInstance = new StateDirectXMan();
		}

		RECT clientRect;
		GetClientRect(g_WindowHandle, &clientRect);

		// Compute the exact client dimensions. This will be used
		// to initialize the render targets for our swap chain.
		unsigned int clientWidth = clientRect.right - clientRect.left;
		unsigned int clientHeight = clientRect.bottom - clientRect.top;

		DXGI_SWAP_CHAIN_DESC swapChainDesc{ 0 };

		// two buffers (front + back)
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = clientWidth;
		swapChainDesc.BufferDesc.Height = clientHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// gets the refresh rate of the monitor (60Hz unless you have a crazy fast monitor)
		swapChainDesc.BufferDesc.RefreshRate = Engine::QueryRefreshRate(clientWidth, clientHeight, vSync);
		// Use the surface or resource as an output render target.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		// output window (in window mode its the front buffer)
		swapChainDesc.OutputWindow = g_WindowHandle;
		// multisample setup
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		// get drop the backbuffer after flip
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		// sub window of screen
		swapChainDesc.Windowed = TRUE;

		UINT createDeviceFlags = createDeviceFlags_define;

		// These are the feature levels that we will accept.
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		// This will be the feature level that 
		// is used to create our device and swap chain.
		D3D_FEATURE_LEVEL featureLevel;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr,
													D3D_DRIVER_TYPE_HARDWARE,
													nullptr,
													createDeviceFlags,
													featureLevels,
													_countof(featureLevels),
													D3D11_SDK_VERSION,
													&swapChainDesc,
													&posInstance->mD3DSwapChain.poD3DSwapChain,
													&posInstance->mD3DDevice.poD3DDevice,
													&featureLevel,
													&posInstance->mD3DDeviceContext.poD3DDeviceContext);

		assert(SUCCEEDED(hr));
	}

	ID3D11Device* StateDirectXMan::GetDevice()
	{
		StateDirectXMan* pMan = StateDirectXMan::privGetInstance();
		assert(pMan);

		return pMan->mD3DDevice.poD3DDevice;
	}

	ID3D11DeviceContext *StateDirectXMan::GetContext()
	{
		StateDirectXMan *pMan = StateDirectXMan::privGetInstance();
		assert(pMan);

		return pMan->mD3DDeviceContext.poD3DDeviceContext;
	}

	IDXGISwapChain* StateDirectXMan::GetSwapChain()
	{
		StateDirectXMan* pMan = StateDirectXMan::privGetInstance();
		assert(pMan);

		return pMan->mD3DSwapChain.poD3DSwapChain;
	}

	void StateDirectXMan::ResizeSwapChain(UINT width, UINT height)
	{
		StateDirectXMan* pMan = StateDirectXMan::privGetInstance();
		assert(pMan);

		//void CleanupRenderTargets()
		//{
		//done	if (g_RenderTargetView) { g_RenderTargetView->Release(); g_RenderTargetView = nullptr; } done
		//done	if (g_DepthStencilView) { g_DepthStencilView->Release(); g_DepthStencilView = nullptr; } done
		//done	if (g_DepthStencilBuffer) { g_DepthStencilBuffer->Release(); g_DepthStencilBuffer = nullptr; }
		//}

		// 解除绑定所有渲染目标（重要）
		//ID3D11RenderTargetView* nullViews[] = { nullptr };
		//g_Context->OMSetRenderTargets(1, nullViews, nullptr);

		HRESULT hr = pMan->mD3DSwapChain.poD3DSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
		if (FAILED(hr))
		{
			assert(false);
		}

		//	Device Recreate
		//	if (g_RenderTargetView)
		//	CreateTexture2D 	if (g_DepthStencilView) 
		//	if (g_DepthStencilBuffer)
		//	OMSetRenderTargets

		//  g_Context->RSSetViewports(1, &g_Viewport); done
	}

	void StateDirectXMan::UpdateViewPort()
	{

	}

	void StateDirectXMan::Destroy()
	{
		delete posInstance;
	}

	StateDirectXMan::StateDirectXMan()
		: mD3DDevice(),
		mD3DDeviceContext(),
		mD3DSwapChain()
	{

	}

	StateDirectXMan::~StateDirectXMan()
	{

	}

	// ------------------------------------------------------
// Find Refresh rate:
// http://www.rastertek.com/dx11tut03.html
// ------------------------------------------------------
	DXGI_RATIONAL StateDirectXMan::QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync)
	{
		DXGI_RATIONAL refreshRate = { 0, 1 };
		if (vsync)
		{
			StateFactory Factory;
			StateAdapter Adapter;
			StateAdapterOutput AdapterOutput;

			DXGI_MODE_DESC* displayModeList;

			// Create a DirectX graphics interface factory.
			HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory.poDXGFactory);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Could not create DXGIFactory instance."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to create DXGIFactory.");
			}

			hr = Factory.poDXGFactory->EnumAdapters(0, &Adapter.poDXGAdapter);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to enumerate adapters."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to enumerate adapters.");
			}

			hr = Adapter.poDXGAdapter->EnumOutputs(0, &AdapterOutput.poDXGAdapterOutput);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to enumerate adapter outputs."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to enumerate adapter outputs.");
			}

			UINT numDisplayModes;
			hr = AdapterOutput.poDXGAdapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr);
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

			hr = AdapterOutput.poDXGAdapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList);
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
		}

		return refreshRate;
	}

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
	StateDirectXMan *StateDirectXMan::privGetInstance()
	{
		// Safety - this forces users to call Create() first before using class
		assert(posInstance != nullptr);

		return posInstance;
	}
}

// --- End of File ---
