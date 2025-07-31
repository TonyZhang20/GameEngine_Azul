//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include "MathEngine.h"
#include "ShaderObject.h"
#include "Render.h"
#include "Mesh.h"
#include "Material.h"

namespace Azul
{
	class GraphicsObject : public Render
	{
	public:
		GraphicsObject(Material* mat, Mesh* mesh);
		
		GraphicsObject(const GraphicsObject&) = delete;
		GraphicsObject& operator = (const GraphicsObject&) = delete;
		virtual ~GraphicsObject() override;

		void SetWorld(Mat4& _world);

		inline virtual void SetGpu(Camera* pCam) override { TransferWorldViewProj(pCam); };
		inline virtual void TransferWorldViewProj(Camera* pCam) { pMat->TransferWorldViewProj(pCam, poWorld); };
		inline virtual void SetShader(ShaderObject* s) { pMat->SetShader(s); }
		inline virtual void SetTexture(TextureObject* t) { pMat->SetTexture(t); }

		inline virtual void BindShader() { assert(pMat); pMat->BindShdaer(); };
		inline virtual void BindTexture() { assert(pMat); pMat->BindTexture(); };
		inline virtual void RestoreState() override {};
		inline virtual void SetState() override {};

		virtual void Render(Camera* pCam);

		/// <summary>
		/// Shader Side - > pShader->ActivateShader(); pShader->ActivateCBV()
		///					 Graphic with Same Shader
		///					 TransferData
		///					 BindTex
		///					 Draw	
		/// </summary>

		virtual void Draw() override;

		Mesh* GetModel() const;
		Mat4& GetWorld();

		inline ShaderObject* GetShader() { assert(pMat); return pMat->GetShader(); };
		inline TextureObject* GetTexture() { assert(pMat); return pMat->GetTexture(); }
	
	protected:
		GraphicsObject();

	protected:
		Material* pMat;
		Mesh* pMesh; //will delete by manager
		Mat4* poWorld; //temp
		
	};

}

#endif

// --- End of File ---
