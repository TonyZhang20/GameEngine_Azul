//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include <Windows.h>
#include <string>
#include <d3d11.h>

#include "TextureObject.h"
#include "Engine.h"
#include "StringThis.h"
#include "StateDirectXMan.h"

#include "DirectXTex.h"

namespace Azul
{

	TextureObject::TextureObject()
		:
		name(TextureObject::Name::Uninitialized),
		poTextureRV(nullptr),
		poSampler(nullptr)
	{

	}

	TextureObject::~TextureObject()
	{
		if(poTextureRV) SafeRelease(poTextureRV);
		if(poSampler) SafeRelease(poSampler);
	}

	TextureObject::TextureObject(TextureObject::Name _name,
								 LPCWSTR filepath,
								 D3D11_FILTER filter)
		:
		name(_name),
		poTextureRV(nullptr),
		poSampler(nullptr)
	{
		HRESULT hr = LoadFromTGAFile(filepath, nullptr, imageInfor);

		if (FAILED(hr)) 
		{
			Trace::out("Failed to load TGA file. HRESULT = 0x%08X\n", hr);
		}

		assert(SUCCEEDED(hr));

		CreateShaderResourceView(StateDirectXMan::GetDevice(),
								imageInfor.GetImage(0, 0, 0),
								imageInfor.GetImageCount(),
								imageInfor.GetMetadata(),
								 &poTextureRV);
		assert(this->poTextureRV);

		// Create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));

		sampDesc.Filter = filter;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = StateDirectXMan::GetDevice()->CreateSamplerState(&sampDesc, &poSampler);

		assert(SUCCEEDED(hr));
		assert(this->poSampler);
	}

	void TextureObject::ActivateTexture()
	{
		StateDirectXMan::GetContext()->PSSetShaderResources((uint32_t) ShaderResourceBufferSlot::TexA, 1, &poTextureRV);
		StateDirectXMan::GetContext()->PSSetSamplers((uint32_t) ShaderSamplerSlot::SampleA, 1, &poSampler);
	}

	void TextureObject::ActivateResourceViewOnly()
	{
		StateDirectXMan::GetContext()->PSSetShaderResources((uint32_t)ShaderResourceBufferSlot::TexA, 1, &poTextureRV);
	}

	void TextureObject::Init(TextureObject::Name _name, LPCWSTR filepath, D3D11_FILTER filter)
	{
		this->name = _name;
		DirectX::ScratchImage testTexture;
		HRESULT hr = LoadFromTGAFile(filepath, nullptr, testTexture);

		if (FAILED(hr))
		{
			Trace::out("Failed to load TGA file. HRESULT = 0x%08X\n", hr);
		}

		assert(SUCCEEDED(hr));

		CreateShaderResourceView(StateDirectXMan::GetDevice(),
			testTexture.GetImage(0, 0, 0),
			testTexture.GetImageCount(),
			testTexture.GetMetadata(),
			&poTextureRV);
		assert(this->poTextureRV);

		CreateSampDesc(filter);
	}

	void TextureObject::Init(ID3D11ShaderResourceView* pSRV, TextureObject::Name _name)
	{
		this->name = _name;
		this->poTextureRV = pSRV;
	}

	//在外面开好了直接传进来
	void TextureObject::SetResourceView(ID3D11ShaderResourceView* pSRV)
	{
		this->poTextureRV = pSRV;
		this->CreateSampDesc();
	}

	void TextureObject::CreateSampDesc(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE address)
	{
		if (this->poSampler)
			SafeRelease(poSampler);
		this->poSampler = nullptr;

		assert(this->poTextureRV);

		HRESULT hr;

		// Create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));

		sampDesc.Filter = filter;
		sampDesc.AddressU = address;
		sampDesc.AddressV = address;
		sampDesc.AddressW = address;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = StateDirectXMan::GetDevice()->CreateSamplerState(&sampDesc, &poSampler);
		assert(SUCCEEDED(hr));
		assert(this->poSampler);
	}

	void TextureObject::SetName(TextureObject::Name _name)
	{
		this->name = _name;
	}

	bool TextureObject::Compare(DLink* pTargetNode)
	{
		return this->name == ((TextureObject*)pTargetNode)->name;
	}

	void TextureObject::Wash()
	{
		this->name = TextureObject::Name::Uninitialized;
	}

	ID3D11ShaderResourceView* TextureObject::GetTextureID() const
	{
		return this->poTextureRV;
	}

	void TextureObject::ReleaseShdaerResource()
	{
		SafeRelease(this->poTextureRV);
	}

	char *TextureObject::GetName()
	{
		// todo - Hack understand why is this needed for print and fix...
		static char pTmp[128];
		strcpy_s(pTmp, 128, StringMe(this->name));
		return pTmp;
	}

}


// --- End of File ---