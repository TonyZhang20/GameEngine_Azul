//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_ConstColor.h"
#include "Engine.h"
#include "StateDirectXMan.h"
#include "CameraNodeManager.h"

namespace Azul
{

	// ---------------------------------------------
	//  Transfer data to the constant buffer
	//    CPU ---> GPU
	//    World, View, Projection Matrix
	// ---------------------------------------------
	GraphicsObject_ConstColor::GraphicsObject_ConstColor(Mesh *mesh,
														 Material* mat,
														 Vec3& LightColor)
		: 
		GraphicsObject(mat, mesh),
		poLightColor(nullptr)
	{
		poLightColor = new Vec3(LightColor);
		assert(poLightColor);
	}

	GraphicsObject_ConstColor::~GraphicsObject_ConstColor()
	{
		delete poLightColor;
	}

	//void GraphicsObject_ConstColor::SetState()
	//{
	//	// Future - settings to directX
	//	// say make it wireframe or change culling mode
	//	// Need to do this properly...
	//	D3D11_RASTERIZER_DESC rasterizerDesc;
	//	memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

	//	rasterizerDesc.AntialiasedLineEnable = FALSE;
	//	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	//	rasterizerDesc.DepthBias = 0;
	//	rasterizerDesc.DepthBiasClamp = 0.0f;
	//	rasterizerDesc.DepthClipEnable = TRUE;
	//	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	//	rasterizerDesc.FrontCounterClockwise = FALSE;
	//	rasterizerDesc.MultisampleEnable = FALSE;

	//	// To Do add scissor rectangle... its faster
	//	rasterizerDesc.ScissorEnable = FALSE;
	//	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	//	// Create the rasterizer state object.
	//	ID3D11RasterizerState *pRasterState;
	//	HRESULT hr;
	//	hr = StateDirectXMan::GetDevice()->CreateRasterizerState(&rasterizerDesc, &pRasterState);
	//	assert(SUCCEEDED(hr));

	//	StateDirectXMan::GetContext()->RSSetState(pRasterState);

	//	SafeRelease(pRasterState);
	//}


	void GraphicsObject_ConstColor::Draw()
	{
		pMesh->RenderIndexBuffer();
	}

	void GraphicsObject_ConstColor::RestoreState()
	{
		//Future work
	}

}

// --- End of File ---
