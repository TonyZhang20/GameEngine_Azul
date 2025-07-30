//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3dcompiler.h>
#include "StateDepthStencilView.h"
#include "StateDirectXMan.h"

namespace Azul
{
	StateDepthStencilView::StateDepthStencilView()
		: poD3DDepthStencilView{nullptr}
	{
	}

	void StateDepthStencilView::Initialize(StateDepthStencilBuffer &r)
	{
		HRESULT hr;
		hr = StateDirectXMan::GetDevice()->CreateDepthStencilView(r.poD3DDepthStencilBuffer, nullptr, &this->poD3DDepthStencilView);
		assert(SUCCEEDED(hr));
	}

	void StateDepthStencilView::Clear(float _clearDepth, uint8_t _clearStencil)
	{
		StateDirectXMan::GetContext()->ClearDepthStencilView(this->poD3DDepthStencilView, 
															  D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
															  _clearDepth, 
															  _clearStencil);

	}

	void StateDepthStencilView::ClearDepthStencilView()
	{
		SafeRelease(this->poD3DDepthStencilView);
		this->poD3DDepthStencilView = nullptr;
	}

	StateDepthStencilView::~StateDepthStencilView()
	{
		SafeRelease(this->poD3DDepthStencilView);
		this->poD3DDepthStencilView = nullptr;
	}

}

// --- End of File ---
