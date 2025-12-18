//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BufferTexture2D.h"
#include "StateDirectXMan.h"

namespace Azul
{

	BufferTexture2D::BufferTexture2D()
		: mWidth{0},
		mHeight{0},
		mFormat{DXGI_FORMAT_UNKNOWN},
		pData{nullptr},
		poTex{nullptr},
		bCreate{false}
	{
	}

	BufferTexture2D::BufferTexture2D(ID3D11Texture2D *p)
		: mWidth { 0 },
		mHeight { 0 },
		mFormat { DXGI_FORMAT_UNKNOWN },
		pData { nullptr },
		poTex { nullptr },
		bCreate { false }
	{
		assert(p);
		this->Initialize(p);
	}


	void BufferTexture2D::Initialize(ID3D11Texture2D *p)
	{
		assert(p);
		this->poTex = p;
	
		D3D11_TEXTURE2D_DESC Desc;
		this->poTex->GetDesc(&Desc);

		this->mFormat = Desc.Format;
		this->mWidth = Desc.Width;
		this->mHeight = Desc.Height;
		this->pData = nullptr;
		this->bCreate = true;
	}

	ID3D11Texture2D *BufferTexture2D::GetID3D11Texture2D()
	{
		assert(this->bCreate == true);
		assert(this->poTex);

		return this->poTex;
	}

	BufferTexture2D::~BufferTexture2D()
	{
		SafeRelease(this->poTex);
	}

}

//--- End of File ---
