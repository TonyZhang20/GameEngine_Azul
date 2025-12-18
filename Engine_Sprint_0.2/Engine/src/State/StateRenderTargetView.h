//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef STATE_RENDER_TARGET_VIEW_H
#define STATE_RENDER_TARGET_VIEW_H

#include <d3d11.h>
#include "MathEngine.h"
#include "StateDepthStencilView.h"

namespace Azul
{
	class StateRenderTargetView
	{
	public:
		StateRenderTargetView();
		StateRenderTargetView(const StateRenderTargetView &) = delete;
		StateRenderTargetView &operator = (const StateRenderTargetView &) = delete;
		virtual ~StateRenderTargetView();
		
		void UnBindAllRenderTarget();
		ID3D11RenderTargetView& const GetD3DRenderTargetView() const;

		void Initialize();
		void Initialize(const unsigned int height, const unsigned int width);
		void Clear(const Vec4 &r);
		void Activate(StateDepthStencilView &r);
		void CleanupRenderTarget();

		void BindTargetView(ID3D11RenderTargetView* targetView);

	public:
		ID3D11RenderTargetView *poD3DRenderTargetView;

	private:
		void privInitialize(ID3D11Texture2D* backBuffer);
	};

}
#endif 

// --- End of File ---
