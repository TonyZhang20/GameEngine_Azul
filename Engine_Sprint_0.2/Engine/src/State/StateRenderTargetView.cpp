//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3dcompiler.h>
#include "StateRenderTargetView.h"
#include "StateDirectXMan.h"
#include "BufferTexture2D.h"

namespace Azul
{
	StateRenderTargetView::StateRenderTargetView()
		: poD3DRenderTargetView{nullptr}
	{

	}

	void StateRenderTargetView::Initialize()
	{
		IDXGISwapChain* pSwapChain = StateDirectXMan::GetSwapChain();
		assert(pSwapChain);

		ID3D11Texture2D* pTexBackBuffer = nullptr;
		HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pTexBackBuffer);
		assert(SUCCEEDED(hr));

		BufferTexture2D backBuffer(pTexBackBuffer);
		this->privInitialize(backBuffer.GetID3D11Texture2D());
	}

	void StateRenderTargetView::Clear(const Vec4 &r)
	{
		StateDirectXMan::GetContext()->ClearRenderTargetView(this->poD3DRenderTargetView, 
															  (const float *)&r);
	}

	void StateRenderTargetView::Activate(StateDepthStencilView &r)
	{
		StateDirectXMan::GetContext()->OMSetRenderTargets(1, 
														   &this->poD3DRenderTargetView,
														   r.poD3DDepthStencilView);
	}

	void StateRenderTargetView::CleanupRenderTarget()
	{
		SafeRelease(this->poD3DRenderTargetView);
	}

	void StateRenderTargetView::BindTargetView(ID3D11RenderTargetView* targetView)
	{
		if (poD3DRenderTargetView)
		{
			CleanupRenderTarget();
		}

		this->poD3DRenderTargetView = targetView;
	}

	void StateRenderTargetView::privInitialize(ID3D11Texture2D* backBuffer)
	{
		ID3D11Device* device = StateDirectXMan::GetDevice();
		
		assert(device);
		assert(backBuffer);

		HRESULT hr = device->CreateRenderTargetView(backBuffer, nullptr, &this->poD3DRenderTargetView);
		assert(SUCCEEDED(hr));
	}

	StateRenderTargetView::~StateRenderTargetView()
	{
		SafeRelease(this->poD3DRenderTargetView);
		this->poD3DRenderTargetView = nullptr;
	}

	void StateRenderTargetView::UnBindAllRenderTarget()
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		StateDirectXMan::GetContext()->OMSetRenderTargets(1,
			nullViews,
			nullptr);
	}

	ID3D11RenderTargetView& const StateRenderTargetView::GetD3DRenderTargetView() const
	{
		return *poD3DRenderTargetView;
	}

}

// --- End of File ---
