
#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include "Entity.h"
#include "Utility.h"
#include "SparseSet.hpp"
#include "EntityManager.h"


namespace zecs
{
	using ComponentID = uint32_t;

	template<typename T>
	struct ComponentData final : IComponentData //final 去虚拟化 优化内存
	{

		virtual void Remove(EntityID entity) final override
		{
			sparseSet.Remove(entity);
		}

		ZSparseSet<T, WORLD_PAGE_SIZE> sparseSet;
	};

	class World
	{
	public:

		~World()
		{
			for (auto itr = componentMap.begin(); itr != componentMap.end(); ++itr)
			{
				delete itr->second;
				itr->second = nullptr;
			}

			componentMap.clear();
		}

		template<typename T>
		inline T& AddComponent(EntityID entity, T&& component)
		{
			ComponentID cid = IndexGetter::Get<T>();
			auto it = componentMap.find(cid);

			if (it == componentMap.end())
			{
				componentMap[cid] = new ComponentData<T>();
			}

			ComponentData<T>* componentData = static_cast< ComponentData<T>* >( componentMap[cid] );
			return componentData->sparseSet.Add(entity, std::forward<T>(component));
		}

		template<typename T>
		inline void RemoveComponent(EntityID entity)
		{
			ComponentID cid = IndexGetter::Get<T>();
			auto it = componentMap.find(cid);

			if (it == componentMap.end()) return;
			//Entity 擦除所有 Component ID
			entities[entity].erase(std::find(entities[entity].begin(), entities[entity].end(), cid));

			//Component 擦除所有 Entity ID
			ComponentData<T>* componentData = static_cast<ComponentData<T>*>(componentMap[cid]);
			componentData->sparseSet.Remove(entity);

			delete ComponentData;
		}

		template <typename... Components>
		bool HasComponent(EntityID entity)
		{
			return HasComponentRecursive<Components...>(entity);
		}

		template <typename... Components>
		inline EntityID SpawnEntity(Components &&...components)
		{
			EntityID entity = EntityManager::Create();
			SpawnEntityRecursive(entity, std::forward<Components>(components)...);
			return entity;
		}

		void DestroyEntity(EntityID entity)
		{
			auto itr = entities.find(entity);
			if (itr == entities.end() || itr->first.version == entity.version)
			{
				return;
			}

			for (auto cid : itr->second)
			{
				componentMap[cid]->Remove(entity);
			}

			EntityManager::Destroy(itr->first);
			entities.erase(itr);
		}

		template <typename... Components>
		ZVector<EntityID> Query()
		{
			return QueryRecursive<Components...>();
		}

		template<typename Component>
		Component& GetComponent(EntityID entity)
		{
			ComponentID cId = IndexGetter::Get<Component>();

			auto it = componentMap.find(cId);

			ComponentData<Component>* componentData = static_cast<ComponentData<Component>*>(it->second);

			return componentData->sparseSet.Get(entity);
		}

	private:
		template <typename T, typename... Remains>
		inline bool HasComponentRecursive(EntityID entity)
		{
			bool hasCurrentComponent = false;

			ComponentID cId = IndexGetter::Get<T>();

			auto it = componentMap.find(cId);
			if (it != componentMap.end())
			{
				ComponentData<T>* componentData = static_cast<ComponentData<T>*>(it->second);
				hasCurrentComponent = componentData->sparseSet.Contain(entity);
			}

			if (!hasCurrentComponent) return false;

			if constexpr (sizeof...(Remains) != 0)
			{
				return HasComponentRecursive<Remains...>(entity);
			}
			else
			{
				return true;
			}
		}

		template <typename T, typename... Remains>
		inline void SpawnEntityRecursive(EntityID entity, T&& component, Remains &&...remains)
		{
			AddComponent(entity, std::forward<T>(component));

			if constexpr (sizeof...(remains) != 0)
			{
				SpawnEntityRecursive(entity, std::forward<Remains>(remains)...);
			}
		}

		template <typename T, typename... Remains>
		inline ZVector<EntityID> QueryRecursive()
		{
			ZVector<EntityID> outEntities;

			auto cid = IndexGetter::Get<T>();
			auto it = componentMap.find(cid);

			if (it == componentMap.end()) return outEntities;

			auto componentData = static_cast<ComponentData<T> *>(it->second);

			for (auto entity : componentData->sparseSet.entities)
			{
				if constexpr (sizeof...(Remains) == 0)
				{
					outEntities.push_back(entity);
				}
				else
				{
					if (HasComponent<Remains...>(entity))
						outEntities.push_back(entity);
				}
			}

			return outEntities;
		}

	private:
		std::unordered_map<ComponentID, IComponentData*> componentMap;
		std::unordered_map<EntityID, ZVector<ComponentID>> entities;
	};
}

#endif // !WORLD_H