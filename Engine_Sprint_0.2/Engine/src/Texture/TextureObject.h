//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef TEXTURE_OBJECT_H
#define TEXTURE_OBJECT_H

#include "DirectXTex.h"
#include <d3d11.h>
#include "DLink.h"

namespace Azul
{
	class TextureObject : public DLink
	{
	public:
		enum class Name
		{
			NullTexture,
			Brick,
			Stone,
			Rocks,
			Duckweed,
			A,
			B,
			C,
			D,
			E,
			CHECKBOARD,
			EMPTY,
			SCENE_WINDOW,
			Uninitialized
		};

	public:
		TextureObject();
		TextureObject(const TextureObject &) = delete;
		TextureObject &operator = (const TextureObject &) = delete;
		virtual ~TextureObject();

		TextureObject(TextureObject::Name _name,
					  LPCWSTR filepath,
					  D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);

		void ActivateTexture();
		void ActivateResourceViewOnly();

		void Init(TextureObject::Name _name,
				  LPCWSTR filepath,
				  D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);

		void Init(ID3D11ShaderResourceView* pSRV,TextureObject::Name _name);

		void CreateSampDesc(D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_MODE address = D3D11_TEXTURE_ADDRESS_WRAP);

		char *GetName();
		void SetName(TextureObject::Name _name);
		void SetResourceView(ID3D11ShaderResourceView* pSRV);
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Wash() override;

		ID3D11ShaderResourceView* GetTextureID() const;
		void ReleaseShdaerResource();

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------
		TextureObject::Name name;

		ID3D11ShaderResourceView *poTextureRV;
		ID3D11SamplerState *poSampler;

		DirectX::ScratchImage imageInfor;

		unsigned int count = 0;

	};

}

#endif

// --- End of File ---
