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
#include "ApplicationEvent.h"
#include "Layer.h"
#include "AnimTimer.h"
#include "BufferFrame.h"

namespace Azul
{
	class Engine : public Layer
	{
	public:
		static const BOOL ENABLE_VSYNC = true;

	public:
		Engine();
		Engine(const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;
		virtual ~Engine();

		virtual void OnAttach() override {};
		virtual void OnDetach() override {};
		virtual void OnUpdate(float UpdateTime) override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		virtual bool LoadContent() = 0;
		virtual void UnloadContent() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;
		virtual	void ClearDepthStencilBuffer() = 0;
		virtual void OnDestory();
		
		virtual void Cleanup();

		int InitDirectX(HINSTANCE hInstance, HWND hwnd, BOOL vSync);
		static DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);


	protected:
		virtual bool OnWindowClose(WindowCloseEvent& e);
		virtual bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		bool quit = false;
		AnimTimer EngineTime;

	public:
		StateRenderTargetView mStateRenderTargetView; //Omset
		//StateRenderTargetView mTextureRenderTarget;

		StateDepthStencilView mDepthStencilView;	//深度模板窗口
		StateDepthStencilBuffer mDepthStencilBuffer; //深度模板
		StateDepthStencil mStateDepthStencil;		//深度模板状态
		StateBlend mBlendStateOff;
		StateBlend mBlendStateAlpha;
		StateRasterizer mStateRasterizerSolid;
		StateRasterizer mStateRasterizerWireframe;
		StateViewport mViewport;

		BufferFrame* poBufferFrame = nullptr;
	};


	// Convenience macro for releasing COM objects.
	#define SafeRelease(x) { if(x){ x->Release(); x = 0; } }
}



#endif

// --- End of File ---
