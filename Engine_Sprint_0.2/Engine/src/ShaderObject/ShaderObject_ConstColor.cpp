//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_ConstColor.h"
#include "Mesh.h"

// User generated headers... 
#include "ConstColor.Px.h"
#include "ConstColor.Vx.h"

namespace Azul
{
	D3D11_INPUT_ELEMENT_DESC ShaderObject_ConstColorDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t) VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ShaderObject_ConstColor::~ShaderObject_ConstColor()
	{

	}

	ShaderObject_ConstColor::ShaderObject_ConstColor(ShaderObject_ConstColor::Name _name)
		: ShaderObject(_name),
		ConstantBuff_Projection { sizeof(Mat4) },
		ConstantBuff_World { sizeof(Mat4) },
		ConstantBuff_View { sizeof(Mat4) },

		ConstantBuff_Color { sizeof(Vec3) },

		VertexShader { sizeof(g_ConstColor_VxShader), (void *) g_ConstColor_VxShader },
		PixelShader { sizeof(g_ConstColor_PxShader),(void *) g_ConstColor_PxShader },
		InputLayout { sizeof(g_ConstColor_VxShader),(void *) g_ConstColor_VxShader,ShaderObject_ConstColorDesc,sizeof(ShaderObject_ConstColorDesc) }
	{

	}

	void ShaderObject_ConstColor::ActivateShader()
	{
		this->VertexShader.SetActive();
		this->InputLayout.SetActive();
		this->PixelShader.SetActive();
	}

	void ShaderObject_ConstColor::ActivateCBV()
	{
		this->ConstantBuff_World.SetActive(ConstantBufferSlot::World);
		this->ConstantBuff_View.SetActive(ConstantBufferSlot::View);
		this->ConstantBuff_Projection.SetActive(ConstantBufferSlot::Projection);
	}

	void ShaderObject_ConstColor::TransferWorldViewProj(Camera *pCam, Mat4 *pWorld)
	{
		assert(pCam);
		assert(pWorld);

		this->ConstantBuff_World.Transfer(pWorld);
		//this->ConstantBuff_View.Transfer(&pCam->getViewMatrix());
		this->ConstantBuff_Projection.Transfer(&pCam->getProjMatrix());
	}

	void ShaderObject_ConstColor::TransferColor(Vec3 *pColor)
	{
		assert(pColor);

		this->ConstantBuff_Color.Transfer(pColor);
	}

}

// --- End of File ---
