// DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Game.h"
#include "File.h"
#include "Application.h"
#include "StateDirectXMan.h"
namespace Azul
{
	//Hack
	extern unsigned int NumIndices;

	void Game::SetDefaultTargetMode()
	{
		//assert(g_d3dDevice);
		//assert(g_d3dDeviceContext);

		//--------------------------------------------------------
		// Set (point to ) the Rasterizers functions to be used
		//--------------------------------------------------------
		//g_d3dDeviceContext->RSSetState(g_d3dRasterizerState);
		this->mStateRasterizerSolid.Activate();

		//--------------------------------------------------------
		// Set (point to ) the Viewport to be used
		//--------------------------------------------------------
		//g_d3dDeviceContext->
		// (1, &g_Viewport);
		this->mViewport.Activate();

		//--------------------------------------------------------
		// Set (point to ) render target
		//      Only one Target, this maps to Pixel shader
		// --------------------------------------------------------
		this->mStateRenderTargetView.Activate(this->mDepthStencilView);
		//StateDirectXMan::GetContext()->OMSetRenderTargets(1, &this->mStateRenderTargetView.poD3DRenderTargetView, nullptr);

		//--------------------------------------------------------
		// Set blend target
		//--------------------------------------------------------
		this->mBlendStateOff.Activate();

		//--------------------------------------------------------
		// Set Depth functions to be used
		//--------------------------------------------------------
		this->mStateDepthStencil.Activate();
	}

	float Game::GetAspectRatio() const
	{
		float ratio = Application::GetWindow()->GetAspectRatio();

		return ratio;
	}

}
