#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include <unordered_map>
#include <vector>
#include "GraphicsObject.h"
#include "ShaderObject.h"
#include "GameObject.h"

namespace Azul
{
	class RenderQueue
	{
	public:
		RenderQueue() = default;
		~RenderQueue() = default;

		RenderQueue(const RenderQueue&) = delete;
		RenderQueue& operator=(const RenderQueue&) = delete;

		void Submit(GameObject* gameObj);
		// Auto sort
		void Submit(GraphicsObject* pObj);

		void Remove(GameObject* gameObj);
		void Remove(GraphicsObject* pObj);
		void Hello();
		//Shader/Material Group
		void RenderAll();

		// 清空一帧的缓存
		void Clear();

	private:
		// 每个 Shader 分一个桶
		std::unordered_map<ShaderObject*, std::vector<GraphicsObject*>> renderBuckets;
	};
}

#endif // RENDER_MAN_H