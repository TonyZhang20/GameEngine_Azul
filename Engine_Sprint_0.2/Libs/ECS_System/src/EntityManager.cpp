#include "EntityManager.h"

namespace zecs
{
	uint32_t EntityManager::curIdx = 0;
	ZVector<EntityID> EntityManager::freeList;
}