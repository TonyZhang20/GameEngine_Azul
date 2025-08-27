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
		/*
		ID3D11Device* device = StateDirectXMan::GetDevice();
		assert(device);

		// 创建2D纹理，绑定渲染目标和着色器资源
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = mWidth;
		texDesc.Height = mHeight;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		HRESULT hr = device->CreateTexture2D(&texDesc, nullptr, &mTexture2D);
		assert(SUCCEEDED(hr));

		// 用StateRenderTargetView初始化RTV（包装mTexture2D）
		mRenderTargetView.CleanupRenderTarget(); // 清理之前RTV
		mRenderTargetView.BindTargetView(nullptr); // 清空指针先

		// 直接调用privInitialize不行，因为它是private，我们复用方式：
		// 这里直接调用设备接口创建RTV，再用BindTargetView设置
		ID3D11RenderTargetView* pRTV = nullptr;
		hr = device->CreateRenderTargetView(mTexture2D, nullptr, &pRTV);
		assert(SUCCEEDED(hr));
		mRenderTargetView.BindTargetView(pRTV);

		//TextureManager::Dump();

		// 创建Shader Resource View
		ID3D11ShaderResourceView* pSRV = nullptr;
		hr = device->CreateShaderResourceView(mTexture2D, nullptr, &pSRV);
		assert(SUCCEEDED(hr));

		//TextureManager::Dump();
		mTextureObject = TextureManager::RequireTexture(TextureObject::Name::SCENE_WINDOW);
		mTextureObject->SetResourceView(pSRV);
		*/

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
