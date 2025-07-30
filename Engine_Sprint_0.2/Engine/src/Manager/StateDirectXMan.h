//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------
#ifndef DIRECT_X_DEVICE_MAN_H
#define DIRECT_X_DEVICE_MAN_H

#include <d3d11.h>

#include "StateDevice.h"
#include "StateDeviceContext.h"
#include "StateSwapChain.h"

#define SafeRelease(x) { if(x){ x->Release(); x = 0; } }

namespace Azul
{
	class StateDirectXMan
	{
	public:

		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------

		static void Create(HWND g_WindowHandle, bool vSync);
		static void Destroy();

		static ID3D11Device *GetDevice();
		static ID3D11DeviceContext *GetContext();
		static IDXGISwapChain* GetSwapChain();
		static void ResizeSwapChain(UINT width, UINT height);
		static void UpdateViewPort();

		~StateDirectXMan();

		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static StateDirectXMan *privGetInstance();

		StateDirectXMan();
		StateDirectXMan(const StateDirectXMan &) = delete;
		StateDirectXMan &operator = (const StateDirectXMan &) = delete;

		static DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);

		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		static StateDirectXMan *posInstance;
		StateDevice        mD3DDevice;
		StateDeviceContext mD3DDeviceContext;
		StateSwapChain     mD3DSwapChain;

	};
}

#endif

// --- End of File ---
