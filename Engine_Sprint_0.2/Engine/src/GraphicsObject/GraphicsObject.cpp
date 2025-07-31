//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject.h"
#include "MathEngine.h"
#include "Mesh.h"
#include "CameraNodeManager.h"
namespace Azul
{
	GraphicsObject::GraphicsObject()
	{
		this->poWorld = new Mat4(Identity);
		this->pMat = nullptr;
		this->pMesh = nullptr;
	}

	GraphicsObject::GraphicsObject(Material* mat, Mesh* mesh)
		: pMat(mat), pMesh(mesh), poWorld(new Mat4(Identity))
	{
	}

	GraphicsObject::~GraphicsObject()
	{
		delete this->poWorld;
	}

	void GraphicsObject::Render(Camera* pCam)
	{
		this->SetState();
		this->SetGpu(pCam); //Passing Data to Gpu
		this->BindTexture();//Passing Texture
		this->Draw();
		this->RestoreState();
	}

	void GraphicsObject::Draw()
	{
		assert(pMesh);
		assert(pMat);
		
		pMesh->ActivateMesh();
		pMesh->RenderIndexBuffer();
	}

	Mesh* GraphicsObject::GetModel() const
	{
		return this->pMesh;
	}

	Mat4& GraphicsObject::GetWorld()
	{
		return *this->poWorld;
	}

	void GraphicsObject::SetWorld(Mat4& _world)
	{
		*this->poWorld = _world;
	}

}



// --- End of File ---
