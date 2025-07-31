//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_LightTexture.h"
#include "TextureManager.h"
#include "CameraNodeManager.h"

namespace Azul
{
	// ---------------------------------------------
	//  Transfer data to the constant buffer
	//    CPU ---> GPU
	//    World, View, Projection Matrix
	// ---------------------------------------------

	GraphicsObject_LightTexture::GraphicsObject_LightTexture(Material* mat, Mesh* mesh, Vec3& lightColor, Vec3& lightPos)
		: GraphicsObject(mat, mesh),
		poLightColor(new Vec3(lightColor)),
		poLightPos(new Vec3(lightPos))
	{

	}

	GraphicsObject_LightTexture::~GraphicsObject_LightTexture()
	{
		delete poLightColor;
		delete poLightPos;
	}

	void GraphicsObject_LightTexture::SetGpu(Camera* pCam)
	{
		GraphicsObject::SetGpu(pCam);

		GetShader()->TransferLightPos(poLightPos);
		GetShader()->TransferColor(poLightColor);
	}

	void GraphicsObject_LightTexture::Draw()
	{
		pMesh->RenderIndexBuffer();
	}

	void GraphicsObject_LightTexture::RestoreState()
	{
		// Future - Undo settings to directX
		Vec3 zeroColor = Vec3(0, 0, 0);
		GetShader()->TransferColor(&zeroColor);
	}

}

// --- End of File ---
