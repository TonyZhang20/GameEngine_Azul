//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef STATE_DEPTH_STENCIL_H
#define STATE_DEPTH_STENCIL_H

#include <d3d11.h>

namespace Azul
{
	class StateDepthStencil
	{
	public:
		StateDepthStencil();
		StateDepthStencil(const StateDepthStencil &) = delete;
		StateDepthStencil &operator = (const StateDepthStencil &) = delete;
		virtual ~StateDepthStencil();

		void Initialize();
		void Activate();
		void CleanupRendeDepthStencil();

		ID3D11DepthStencilState *poD3dDepthStencilState;
	};

}
#endif 

// --- End of File ---
