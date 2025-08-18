//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <d3d11.h>
#include "MathEngine.h"
#include "Camera.h"

namespace Azul
{
	class Camera;
	class Mat4;

	class ShaderObject
	{
	public:
		enum class Name
		{
			NullShader,
			ColorByVertex,
			FlatTexture,
			LightTexture,
			ConstColor,
			Uninitialized
		};

	public:
		ShaderObject() = delete;
		ShaderObject(const ShaderObject&) = delete;
		ShaderObject& operator = (const ShaderObject&) = delete;
		virtual ~ShaderObject();

		ShaderObject(ShaderObject::Name _name);

		virtual void ActivateShader() = 0;
		virtual void ActivateCBV() = 0; //Constant Shdaer View -> Active Slot
		virtual void TransferWorldViewProj(Camera* pCam, Mat4* pWorld) = 0; //Trans Value

		virtual void TransferProj(Mat4& project) = 0;
		virtual void TransferView(Mat4& view) = 0;
		virtual void TransferWorld(Mat4& pWorld) = 0;


		virtual void TransferLightPos(Vec3* pLightPos);
		virtual void TransferColor(Vec3* pColor);

		static const char* GetLatestProfile_PixelShader();
		static const char* GetLatestProfile_VertexShader();

		char* GetName();
		void SetName(ShaderObject::Name _name);

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------
		ShaderObject::Name name;
	};
}


#endif

// --- End of File ---
