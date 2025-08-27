//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3dcompiler.h>
#include "StateViewport.h"
#include "StateDirectXMan.h"

namespace Azul
{
	StateViewport::StateViewport()
		: poD3DViewport{nullptr}
	{
	}

	void StateViewport::Initialize(int _width, int _height)
	{
		this->poD3DViewport = new D3D11_VIEWPORT();
		assert(this->poD3DViewport);

		poD3DViewport->Width = static_cast<float>(_width);
		poD3DViewport->Height = static_cast<float>(_height);
		poD3DViewport->TopLeftX = 0.0f;
		poD3DViewport->TopLeftY = 0.0f;
		poD3DViewport->MinDepth = 0.0f;
		poD3DViewport->MaxDepth = 1.0f;
	}

	void StateViewport::Activate()
	{
		assert(this->poD3DViewport);
		StateDirectXMan::GetContext()->RSSetViewports(1, this->poD3DViewport);
	}

	void StateViewport::ResizeViewPort(int _width, int _height)
	{
		assert(this->poD3DViewport);

		poD3DViewport->Width = static_cast<float>(_width);
		poD3DViewport->Height = static_cast<float>(_height);
	}

	StateViewport::~StateViewport()
	{
		delete this->poD3DViewport;
		this->poD3DViewport = nullptr;
	}

}

// --- End of File ---
