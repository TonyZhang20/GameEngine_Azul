//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------
#ifndef ENGINE_H
#define ENGINE_H

#include "WindowsWindow.h"

#include <d3d11.h>
#include "StateDepthStencil.h"
#include "StateRasterizer.h"
#include "StateBlend.h"
#include "StateViewport.h"
#include "StateDepthStencilBuffer.h"
#include "StateDepthStencilView.h"
#include "StateRenderTargetView.h"

namespace Azul
{
	class Engine
	{
	public:
		static const BOOL ENABLE_VSYNC = true;

	public:
		Engine(const char* pName, int width, int height);

		Engine() = delete;
		Engine(const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;
		virtual ~Engine();

		virtual Vec4 GetWindowColor();

		virtual bool LoadContent() = 0;
		virtual void UnloadContent() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;
		virtual	void ClearDepthStencilBuffer() = 0;

		void Cleanup();

		int InitDirectX(HINSTANCE hInstance, BOOL vSync);
		int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow);

		void Present(bool vSync);
		static DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);
		int Run();

		StateRenderTargetView mStateRenderTargetView;
		StateDepthStencilView mDepthStencilView;
		StateDepthStencilBuffer mDepthStencilBuffer;
		StateBlend mBlendStateOff;
		StateBlend mBlendStateAlpha;
		StateDepthStencil mStateDepthStencil;
		StateRasterizer mStateRasterizerSolid;
		StateRasterizer mStateRasterizerWireframe;
		StateViewport mViewport;

		D3D11_VIEWPORT g_Viewport;

		const char* pName;
		const int mWindowWidth;
		const int mWindowHeight;

		WindowsWindow* pWindow = nullptr;
	};


	// Convenience macro for releasing COM objects.
	#define SafeRelease(x) { if(x){ x->Release(); x = 0; } }
}



#endif

// --- End of File ---
