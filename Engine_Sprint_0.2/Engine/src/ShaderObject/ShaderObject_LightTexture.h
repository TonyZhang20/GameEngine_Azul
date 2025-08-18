//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_LIGHT_TEXTURE_H
#define SHADER_OBJECT_LIGHT_TEXTURE_H

#include "ShaderObject.h"
#include "BufferCBV_vs.h"
#include "BufferVertexShader.h"
#include "BufferPixelShader.h"
#include "BufferInputLayout.h"

namespace Azul
{
	class ShaderObject_LightTexture : public ShaderObject
	{
	public:
		ShaderObject_LightTexture() = delete;
		ShaderObject_LightTexture(const ShaderObject_LightTexture &) = delete;
		ShaderObject_LightTexture &operator = (const ShaderObject_LightTexture &) = delete;
		~ShaderObject_LightTexture();

		ShaderObject_LightTexture(ShaderObject_LightTexture::Name _name);

		virtual void ActivateShader() override;
		virtual void ActivateCBV() override;
		virtual void TransferWorldViewProj(Camera *pCam, Mat4 *pWorld) override;
		virtual void TransferLightPos(Vec3* pLightPos) override;
		virtual void TransferColor(Vec3* pColor) override;

		virtual void TransferProj(Mat4& project) override;
		virtual void TransferView(Mat4& view)    override;
		virtual void TransferWorld(Mat4& pWorld) override;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		BufferCBV_vs ConstantBuff_Projection;
		BufferCBV_vs ConstantBuff_World;
		BufferCBV_vs ConstantBuff_View;

		BufferCBV_vs ConstantBuff_Color;
		BufferCBV_vs ConstantBuff_LightPos;

		BufferVertexShader VertexShader;
		BufferPixelShader  PixelShader;
		BufferInputLayout  InputLayout;
	};

}
#endif

// --- End of File ---
