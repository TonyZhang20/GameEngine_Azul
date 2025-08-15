#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "ZVector.hpp"
#include "Entity.h"

namespace zecs
{
	/// <summary>
	/// 主要用于创建与回收Entity ID
	/// </summary>
	class EntityManager final
	{
	public:
		static EntityID Create()
		{
			if (!freeList.empty())
			{
				EntityID id = freeList.back(); //Create a copy
				freeList.pop_back();
				id.version++;
				return id;
			}

			return EntityID{ curIdx++, 0 };
		}

		static void Destroy(EntityID id)
		{
			freeList.push_back(id);
		}

	private:
		inline static ZVector<EntityID> freeList;
		inline static uint32_t curIdx = 0;
	};
}

#endif // !ENTITY_MANAGER_H