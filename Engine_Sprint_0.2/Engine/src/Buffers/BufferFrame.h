#ifndef BUFFER_FRAME_H
#define BUFFER_FRAME_H

#include <d3d11.h>
#include "MathEngine.h"
#include "StateDepthStencilBuffer.h"
#include "StateDepthStencil.h"
#include "StateRenderTargetView.h"
#include "BufferTexture2D.h"
#include "TextureObject.h"      // 加入TextureObject管理SRV

namespace Azul
{
	class BufferFrame
	{
	public:
		BufferFrame();
		~BufferFrame();

		BufferFrame(const BufferFrame&) = delete;
		BufferFrame& operator=(const BufferFrame&) = delete;

		// 创建离屏资源（纹理，RTV，SRV，TextureObject）
		void Create();

		// 设置为当前渲染目标
		void SetActive();

		// 恢复默认渲染目标（回到swap chain）
		void Restore();

		void OnResize();

		// 获取TextureObject指针，用于ImGui::Image
		TextureObject* GetTextureObject() const;

	private:

		ID3D11Texture2D* mTexture2D;             // 离屏纹理
		TextureObject* mTextureObject;           // 包含SRV和采样器

		//StateRenderTargetView mRenderTargetView; // RTV包装类
	};
}

#endif