#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "ZVector.hpp"
#include "Entity.h"

namespace zecs
{
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
		static ZVector<EntityID> freeList;
		static uint32_t curIdx;
	};
}

#endif // !ENTITY_MANAGER_H