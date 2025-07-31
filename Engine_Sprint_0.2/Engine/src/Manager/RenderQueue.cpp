#include "RenderQueue.h"
#include "GraphicsObject.h"
#include "ShaderObject.h"
#include "CameraNodeManager.h"

namespace Azul
{
	void RenderQueue::Submit(GameObject* gameObj)
	{
		GraphicsObject* pObj = gameObj->GetGraphicsObject();

		assert(pObj);

		ShaderObject* pShader = pObj->GetShader();
		assert(pShader);

		// 按 Shader 分组：插入对应的 bucket
		renderBuckets[pShader].push_back(pObj);
	}

	void RenderQueue::Submit(GraphicsObject* pObj)
	{
		assert(pObj);

		ShaderObject* pShader = pObj->GetShader();
		assert(pShader);

		// 按 Shader 分组：插入对应的 bucket
		renderBuckets[pShader].push_back(pObj);
	}

	void RenderQueue::Remove(GameObject* gameObj)
	{
		if (gameObj && gameObj->GetGraphicsObject())
		{
			Remove(gameObj->GetGraphicsObject());
		}
	}

	void RenderQueue::Remove(GraphicsObject* pObj)
	{
		ShaderObject* pShader = pObj->GetShader();

		auto it = renderBuckets.find(pShader);
		if (it != renderBuckets.end())
		{
			auto& vec = it->second;
			auto objIt = std::find(vec.begin(), vec.end(), pObj);
			if (objIt != vec.end())
			{
				vec.erase(objIt);
			}
		}
	}

	void RenderQueue::RenderAll()
	{
		// 遍历每个 Shader 的渲染桶
		for (auto& bucket : renderBuckets)
		{
			ShaderObject* pShader = bucket.first;
			std::vector<GraphicsObject*>& objList = bucket.second;

			// 绑定 Shader（一次）
			pShader->ActivateShader();
			pShader->ActivateCBV(); //public const buffer，like Light、ViewProj

			for (GraphicsObject* pObj : objList)
			{
				assert(pObj);
				pObj->SetState();

				// Set Transform
				pObj->TransferWorldViewProj(CameraNodeManager::GetMainCam());

				//Default check if Texture
				pObj->BindTexture();

				pObj->Draw();

				pObj->RestoreState();
			}
			
		}
	}

	void RenderQueue::Clear()
	{
		// 清空每个 bucket 的对象列表
		for (auto& bucket : renderBuckets)
		{
			bucket.second.clear();
		}
	}
}