//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "Engine.h"
#include "ShaderObject.h"
#include "StringThis.h"
#include "StateDirectXMan.h"

namespace Azul
{

	ShaderObject::ShaderObject(ShaderObject::Name _name)
		:
		name(_name)
	{

	}

	ShaderObject::~ShaderObject()
	{

	}

	void ShaderObject::SetName(ShaderObject::Name _name)
	{
		this->name = _name;
	}

	char* ShaderObject::GetName()
	{
		// todo - Hack understand why is this needed for print and fix...
		static char pTmp[128];
		strcpy_s(pTmp, 128, StringMe(this->name));
		return pTmp;
	}

	// -----------------------------------------------------------
	// Get the latest profile for the specified vertex shader type.
	// -----------------------------------------------------------
	const char* ShaderObject::GetLatestProfile_VertexShader()
	{
		// Query the current feature level:
		D3D_FEATURE_LEVEL featureLevel = StateDirectXMan::GetDevice()->GetFeatureLevel();

		switch (featureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
		{
			return "vs_5_0";
		}
		break;

		case D3D_FEATURE_LEVEL_10_1:
		{
			return "vs_4_1";
		}
		break;

		case D3D_FEATURE_LEVEL_10_0:
		{
			return "vs_4_0";
		}
		break;

		case D3D_FEATURE_LEVEL_9_3:
		{
			return "vs_4_0_level_9_3";
		}
		break;

		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_1:
		{
			return "vs_4_0_level_9_1";
		}
		break;

		default:
			assert(false);

		}

		return "";
	}


	void ShaderObject::TransferLightPos(Vec3* pLightPos)
	{
		// if you need it it should be overrided in derived object
		assert(false);

		AZUL_UNUSED_VAR(pLightPos);
	}

	void ShaderObject::TransferColor(Vec3* pColor)
	{
		assert(false);

		AZUL_UNUSED_VAR(pColor);
	}

	// -----------------------------------------------------------
	// Get the latest profile for the specified pixel shader type.
	// -----------------------------------------------------------
	const char* ShaderObject::GetLatestProfile_PixelShader()
	{
		// Query the current feature level:
		D3D_FEATURE_LEVEL featureLevel = StateDirectXMan::GetDevice()->GetFeatureLevel();
		switch (featureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
		{
			return "ps_5_0";
		}
		break;
		case D3D_FEATURE_LEVEL_10_1:
		{
			return "ps_4_1";
		}
		break;
		case D3D_FEATURE_LEVEL_10_0:
		{
			return "ps_4_0";
		}
		break;
		case D3D_FEATURE_LEVEL_9_3:
		{
			return "ps_4_0_level_9_3";
		}
		break;
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_1:
		{
			return "ps_4_0_level_9_1";
		}
		break;
		}
		return "";
	}

}


// --- End of File ---