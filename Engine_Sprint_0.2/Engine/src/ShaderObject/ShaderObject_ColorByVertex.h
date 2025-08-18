//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_COLOR_BY_VERTEX_H
#define SHADER_OBJECT_COLOR_BY_VERTEX_H

#include "ShaderObject.h"
#include "BufferCBV_vs.h"
#include "BufferVertexShader.h"
#include "BufferPixelShader.h"
#include "BufferInputLayout.h"

namespace Azul
{
	class ShaderObject_ColorByVertex : public ShaderObject
	{
	public:

	public:
		ShaderObject_ColorByVertex() = delete;
		ShaderObject_ColorByVertex(const ShaderObject_ColorByVertex&) = delete;
		ShaderObject_ColorByVertex& operator = (const ShaderObject_ColorByVertex&) = delete;
		~ShaderObject_ColorByVertex();

		ShaderObject_ColorByVertex(ShaderObject_ColorByVertex::Name _name);

		virtual void ActivateShader() override;
		virtual void ActivateCBV() override;
		virtual void TransferWorldViewProj(Camera* pCam, Mat4* pWorld) override;

		virtual void TransferProj(Mat4& project) override;
		virtual void TransferView(Mat4& view) override;
		virtual void TransferWorld(Mat4& pWorld) override;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------
		// 
		// Shader resources
		BufferCBV_vs ConstantBuff_Projection;
		BufferCBV_vs ConstantBuff_World;
		BufferCBV_vs ConstantBuff_View;

		BufferVertexShader VertexShader;
		BufferPixelShader  PixelShader;
		BufferInputLayout  InputLayout;
	};

}

#endif

// --- End of File ---
