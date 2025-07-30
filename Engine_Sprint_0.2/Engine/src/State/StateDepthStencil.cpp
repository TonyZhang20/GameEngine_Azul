//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3dcompiler.h>
#include "StateDepthStencil.h"
#include "StateDirectXMan.h"

namespace Azul
{
	StateDepthStencil::StateDepthStencil()
		:poD3dDepthStencilState{nullptr}
	{
	}

	void StateDepthStencil::Initialize()
	{
		// Setup depth/stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{0};

		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// not using any stencil... its disabled
		depthStencilStateDesc.StencilEnable = FALSE;

		HRESULT hr;
		hr = StateDirectXMan::GetDevice()->CreateDepthStencilState(&depthStencilStateDesc, &this->poD3dDepthStencilState);
		assert(SUCCEEDED(hr));
	}

	void StateDepthStencil::Activate()
	{
		assert(this->poD3dDepthStencilState);
		StateDirectXMan::GetContext()->OMSetDepthStencilState(this->poD3dDepthStencilState, 1);
	}

	void StateDepthStencil::CleanupRendeDepthStencil()
	{
		SafeRelease(this->poD3dDepthStencilState);
		this->poD3dDepthStencilState = nullptr;
	}

	StateDepthStencil::~StateDepthStencil()
	{
		SafeRelease(this->poD3dDepthStencilState);
		this->poD3dDepthStencilState = nullptr;
	}

}

// --- End of File ---
