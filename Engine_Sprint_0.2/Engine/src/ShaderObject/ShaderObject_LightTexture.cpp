//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_LightTexture.h"
#include "Mesh.h"
#include "StateDirectXMan.h"

// User generated headers... 
#include "LightTexture.Px.h"
#include "LightTexture.Vx.h"

namespace Azul
{
	D3D11_INPUT_ELEMENT_DESC ShaderObject_LightTextureDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t) VertexSlot::Position, offsetof(VertexPos, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t) VertexSlot::TexCoord, offsetof(VertexTexCoord, TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t) VertexSlot::Norm, offsetof(VertexNorm, Norm),  D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};

	ShaderObject_LightTexture::~ShaderObject_LightTexture()
	{

	}

	ShaderObject_LightTexture::ShaderObject_LightTexture(ShaderObject_LightTexture::Name _name)
		: ShaderObject(_name),
		ConstantBuff_Projection { sizeof(Mat4) },
		ConstantBuff_World { sizeof(Mat4) },
		ConstantBuff_View { sizeof(Mat4) },

		ConstantBuff_Color { sizeof(Vec3) },
		ConstantBuff_LightPos { sizeof(Vec3) },

		VertexShader { sizeof(g_LightTexture_VxShader), (void *) g_LightTexture_VxShader },
		PixelShader { sizeof(g_LightTexture_PxShader),(void *) g_LightTexture_PxShader },
		InputLayout { sizeof(g_LightTexture_VxShader),(void *) g_LightTexture_VxShader, ShaderObject_LightTextureDesc,sizeof(ShaderObject_LightTextureDesc) }

	{

	}

	void ShaderObject_LightTexture::ActivateShader()
	{
		this->VertexShader.SetActive();
		this->InputLayout.SetActive();
		this->PixelShader.SetActive();
	}

	void ShaderObject_LightTexture::ActivateCBV()
	{
		this->ConstantBuff_World.SetActive(ConstantBufferSlot::World);
		this->ConstantBuff_View.SetActive(ConstantBufferSlot::View);
		this->ConstantBuff_Projection.SetActive(ConstantBufferSlot::Projection);

		this->ConstantBuff_Color.SetActive(ConstantBufferSlot::Color);
		this->ConstantBuff_LightPos.SetActive(ConstantBufferSlot::LightPos);
	}

	void ShaderObject_LightTexture::TransferWorldViewProj(Camera *pCam, Mat4 *pWorld)
	{
		assert(pCam);
		assert(pWorld);

		this->ConstantBuff_World.Transfer(pWorld);
		//this->ConstantBuff_View.Transfer(&pCam->getViewMatrix());
		this->ConstantBuff_Projection.Transfer(&pCam->getProjMatrix());
	}

	void ShaderObject_LightTexture::TransferLightPos(Vec3* pLightPos)
	{
		assert(pLightPos);

		this->ConstantBuff_LightPos.Transfer(pLightPos);
	}

	void ShaderObject_LightTexture::TransferColor(Vec3* pColor)
	{
		assert(pColor);

		this->ConstantBuff_Color.Transfer(pColor);
	}

	void ShaderObject_LightTexture::TransferProj(Mat4& project)
	{
		this->ConstantBuff_Projection.Transfer(&project);
	}

	void ShaderObject_LightTexture::TransferView(Mat4& view)
	{
		this->ConstantBuff_View.Transfer(&view);
	}

	void ShaderObject_LightTexture::TransferWorld(Mat4& pWorld)
	{
		this->ConstantBuff_World.Transfer(&pWorld);
	}


}

// --- End of File ---
