#include "BufferFrame.h"
#include "StateDirectXMan.h"
#include "TextureManager.h"
#include <cassert>

namespace Azul
{
	BufferFrame::BufferFrame()
		: mTexture2D(nullptr), mTextureObject(nullptr)
	{
		Create();
	}

	BufferFrame::~BufferFrame()
	{
		SafeRelease(mTexture2D);
	}

	void BufferFrame::Create()
	{
		if (mTexture2D != nullptr)
		{
			SafeRelease(mTexture2D);
		}

		IDXGISwapChain* pSwapChain = StateDirectXMan::GetSwapChain();
		assert(pSwapChain);

		ID3D11Texture2D* pTexBackBuffer = nullptr;
		HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pTexBackBuffer);
		assert(SUCCEEDED(hr));

		BufferTexture2D backBuffer(pTexBackBuffer);

		ID3D11Texture2D* pBackBuffer = backBuffer.GetID3D11Texture2D();

		D3D11_TEXTURE2D_DESC backDesc = {};
		pBackBuffer->GetDesc(&backDesc);

		D3D11_TEXTURE2D_DESC srvDesc = backDesc;
		srvDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		srvDesc.CPUAccessFlags = 0;
		srvDesc.MiscFlags = 0;
		srvDesc.Usage = D3D11_USAGE_DEFAULT;

		hr = StateDirectXMan::GetDevice()->CreateTexture2D(&srvDesc, nullptr, &mTexture2D);
		assert(SUCCEEDED(hr));

		//Copy -> TODO: Change into Render here first and back after
		StateDirectXMan::GetContext()->CopyResource(this->mTexture2D, pBackBuffer);

		//ReCreate Target View
		ID3D11ShaderResourceView* pSRV = nullptr;
		hr = StateDirectXMan::GetDevice()->CreateShaderResourceView(mTexture2D, nullptr, &pSRV);
		assert(SUCCEEDED(hr));

		//映射目标
		mTextureObject = TextureManager::RequireTexture(TextureObject::Name::SCENE_WINDOW);
		mTextureObject->SetResourceView(pSRV);
	}

	//void BufferFrame::SetActive(StateDepthStencilView& DepthStencil)
	//{
	//	mRenderTargetView.Activate(DepthStencil);
	//}

	void BufferFrame::SetActive()
	{
		//Copy Resources

		IDXGISwapChain* pSwapChain = StateDirectXMan::GetSwapChain();
		assert(pSwapChain);

		ID3D11Texture2D* pTexBackBuffer = nullptr;
		HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pTexBackBuffer);
		assert(SUCCEEDED(hr));

		BufferTexture2D backBuffer(pTexBackBuffer);

		StateDirectXMan::GetContext()->CopyResource(this->mTexture2D, backBuffer.GetID3D11Texture2D());
	}

	void BufferFrame::OnResize()
	{
		Create();
	}


	void BufferFrame::Restore()
	{

	}

	TextureObject* BufferFrame::GetTextureObject() const
	{
		return mTextureObject;
	}
}
