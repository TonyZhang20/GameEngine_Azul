//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_FlatTexture.h"
#include "Mesh.h"
#include "StateDirectXMan.h"

// User generated headers... 
#include "FlatTexture.Px.h"
#include "FlatTexture.Vx.h"

namespace Azul
{
	D3D11_INPUT_ELEMENT_DESC ShaderObject_FlatTextureDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t) VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t)VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ShaderObject_FlatTexture::~ShaderObject_FlatTexture()
	{
	}

	ShaderObject_FlatTexture::ShaderObject_FlatTexture(ShaderObject_FlatTexture::Name _name)
		: ShaderObject(_name),
		ConstantBuff_Projection { sizeof(Mat4) },
		ConstantBuff_World { sizeof(Mat4) },
		ConstantBuff_View { sizeof(Mat4) },
		VertexShader { sizeof(g_FlatTexture_VxShader), (void *) g_FlatTexture_VxShader },
		PixelShader { sizeof(g_FlatTexture_PxShader),(void *) g_FlatTexture_PxShader },
		InputLayout { sizeof(g_FlatTexture_VxShader),(void *) g_FlatTexture_VxShader, ShaderObject_FlatTextureDesc,sizeof(ShaderObject_FlatTextureDesc) }
	{
	}

	void ShaderObject_FlatTexture::ActivateShader()
	{
		this->VertexShader.SetActive();
		this->InputLayout.SetActive();
		this->PixelShader.SetActive();
	}

	void ShaderObject_FlatTexture::ActivateCBV()
	{
		this->ConstantBuff_World.SetActive(ConstantBufferSlot::World);
		this->ConstantBuff_View.SetActive(ConstantBufferSlot::View);
		this->ConstantBuff_Projection.SetActive(ConstantBufferSlot::Projection);
	}

	void ShaderObject_FlatTexture::TransferWorldViewProj(Camera* pCam, Mat4* pWorld)
	{
		assert(pCam);
		assert(pWorld);

		this->ConstantBuff_World.Transfer(pWorld);
		//this->ConstantBuff_View.Transfer(&pCam->getViewMatrix());
		this->ConstantBuff_Projection.Transfer(&pCam->getProjMatrix());
	}

	void ShaderObject_FlatTexture::TransferProj(Mat4& project)
	{
		this->ConstantBuff_Projection.Transfer(&project);
	}

	void ShaderObject_FlatTexture::TransferView(Mat4& view)
	{
		this->ConstantBuff_View.Transfer(&view);
	}

	void ShaderObject_FlatTexture::TransferWorld(Mat4& pWorld)
	{
		this->ConstantBuff_World.Transfer(&pWorld);
	}


}

// --- End of File ---
