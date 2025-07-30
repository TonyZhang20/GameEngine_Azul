//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef STATE_DEPTH_STENCIL_BUFFER_H
#define STATE_DEPTH_STENCIL_BUFFER_H

#include <d3d11.h>

namespace Azul
{
	class StateDepthStencilBuffer
	{
	public:
		StateDepthStencilBuffer();
		StateDepthStencilBuffer(const StateDepthStencilBuffer &) = delete;
		StateDepthStencilBuffer &operator = (const StateDepthStencilBuffer &) = delete;
		virtual ~StateDepthStencilBuffer();

		void Initialize(uint32_t clientWidth, uint32_t clientHeight);
		void ClearDepthStencilBuffer();

		ID3D11Texture2D *poD3DDepthStencilBuffer;
	};

}
#endif 

// --- End of File ---
