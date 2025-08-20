#ifndef ARCHETYPE_WORLD_H
#define ARCHETYPE_WORLD_H

#include "Chunk.h"
#include "Utility.h"
#include "Archetype.h"
#include "ZVector.hpp"
#include "EntityManager.h"
#include <unordered_map>
#include <algorithm>

namespace zecs
{
	class ArchetypeWorld
	{
		using ComponentTuple = std::tuple<Archetype*, Chunk*, size_t>;
		using ComponentLayoutMap = std::unordered_map<ComponentID, ComponentLayout>;

	public:

		template <typename T>
		T& GetComponent(EntityID entity)
		{
			auto [archetype, chunk, index] = entities[entity];
			return chunk->GetComponentArray<T>()[index];
		}

		template <typename T>
		bool HasComponent(EntityID entity)
		{
			if (auto it = entities.find(entity); it == entities.end()) return false;
			auto [archetype, chunk, index] = entities[entity];
			auto cid = IndexGetter::Get<T>();
			return archetype->componentLayouts.find(cid) != archetype->componentLayouts.end();
		}

		template<typename... Components>
		ZVector<ComponentID> GetComponentTypes()
		{
			ZVector<ComponentID> out;
			GetComponentRecursive<Components...>(out);
			return out;
		}

		template<typename T, typename... Rest>
		inline void GetComponentRecursive(ZVector<ComponentID>& v)
		{
			v.push_back(IndexGetter::Get<T>());
			if constexpr (sizeof...(Rest) > 0)
				GetComponentRecursive<Rest...>(v);
		}
		
		template <typename... Components>
		inline ComponentLayoutMap CalculateLayout()
		{
			ComponentLayoutMap map;

			CalculateLayoutRecursive<Components...>(map);

			return map;
		}

		template <typename T ,typename... Rest>
		inline void CalculateLayoutRecursive(ComponentLayoutMap& map)
		{
			using DecayedT = std::decay_t<T>;

			auto cid = IndexGetter::Get<DecayedT>();
			
			if (map.find(cid) == map.end())
			{
				map[cid] = ComponentLayout(sizeof(DecayedT), alignof(DecayedT));
				map[cid].RegisterLayout<DecayedT>();
			}

			if constexpr (sizeof...(Rest) > 0)
			{
				CalculateLayoutRecursive<Rest...>(map);
			}
		}

		template <typename... Components>
		EntityID SpawnEntity(Components &&...components)
		{
			EntityID entity = EntityManager::Create();
			ZVector<ComponentID> componentTypes = GetComponentTypes<Components...>();

			std::sort(componentTypes.begin(), componentTypes.end());

			size_t hash = Archetype::HashComponentTypes(componentTypes);

			if (auto it = archetypes.find(hash); it == archetypes.end()) // 没有对应Archetype则新创建
			{
				std::unordered_map<ComponentID, ComponentLayout> componentLayouts = CalculateLayout<Components...>();
				CreateArchetype(hash, componentLayouts);
			}

			auto [chunk, index] = archetypes[hash].AddEntity(entity, std::forward<Components>(components)...);

			entities[entity] = { &archetypes[hash], chunk, index };

			return entity;
		}

		void DestroyEntity(EntityID entity)
		{
			if (auto it = entities.find(entity); it != entities.end() && it->first.version == entity.version)
			{
				auto [archetype, chunk, index] = entities[entity];

				EntityID changedEntity = chunk->RemoveEntity(index);

				if (changedEntity.index != 0xffffff)
				{
					entities[changedEntity] = { archetype, chunk, index };
				}

				entities.erase(entity);
				EntityManager::Destroy(entity);
			}
		}

		void CreateArchetype(size_t hash, std::unordered_map<ComponentID, ComponentLayout> componentLayouts)
		{
			archetypes.try_emplace(hash, hash, componentLayouts);
		}

		template<typename T>
		inline T& AddComponent(EntityID entity, T&& component)
		{
			using DecayedT = std::decay_t<T>;

			ComponentID cid = IndexGetter::Get<T>();
			auto [archetype, chunk, index] = entities[entity];
			ZVector<ComponentID> newComponentTypes = archetype->componentTypes;

			newComponentTypes.push_back(cid);

			std::sort(newComponentTypes.begin(), newComponentTypes.end());

			size_t hash = Archetype::HashComponentTypes(newComponentTypes);

			//if doesnt have entity, create one
			if (auto it = archetypes.find(hash); it == archetypes.end())
			{
				auto newComponentLayouts = archetype->componentLayouts;

				//Create new layout
				newComponentLayouts[cid] = ComponentLayout{ 0, sizeof(DecayedT), alignof(DecayedT) };
				newComponentLayouts[cid].RegisterLayout<DecayedT>();

				//创建对应的archetype
				CreateArchetype(hash, newComponentLayouts);
			}

			//取地址，如果更改也会对应更改
			Archetype* destArchetype = &archetypes[hash];

			// 分配好entity到对应Chunk，并得到索引
			auto [destChunk, destIndex] = destArchetype->AllocateEntity(entity);

			// 拷贝已有的数据 to destChunk
			CopyComponents(archetype, chunk, index, destArchetype, destChunk, destIndex);

			// 拷贝新增的Component的数据
			destChunk->CopyComponentData(destIndex, std::forward<T>(component));

			// 原Chunk中删除
			chunk->RemoveEntity(index);

			// 更新映射关系
			entities[entity] = { destArchetype, destChunk, destIndex };

			return destChunk->GetComponentArray<T>()[index];
		}

		template <typename T>
		inline void RemoveComponent(EntityID entity)
		{
			auto cid = IndexGetter::Get<T>();
			auto [archetype, chunk, index] = entities[entity];
			auto newComponentTypes = archetype->componentTypes;

			newComponentTypes.erase(cid);

			std::sort(newComponentTypes.begin(), newComponentTypes.end());

			size_t hash = Archetype::HashComponentTypes(newComponentTypes);

			if (auto it = archetypes.find(hash); it == archetypes.end())
			{
				auto newComponentLayouts = archetype->componentLayouts;
				newComponentLayouts.erase(cid);
				CreateArchetype(hash, newComponentLayouts);
			}

			Archetype* destArchetype = &archetypes[hash];
			auto [destChunk, destIndex] = destArchetype->AllocateEntity(entity);

			CopyComponents(archetype, chunk, index, destArchetype, destChunk, destIndex);

			chunk->RemoveEntity(index);

			entities[entity] = { destArchetype, destChunk, destIndex };
		}

		inline void CopyComponents(Archetype* src, Chunk* srcChunk, size_t srcIndex,
			Archetype* dest, Chunk* destChunk, size_t destIndex)
		{
			//从 src 复制 到 dest
			for (auto& [type, _] : src->componentLayouts)
			{
				if (dest->componentLayouts.find(type) != dest->componentLayouts.end()) // 两者共有的component
				{
					// 获取组件大小
					size_t compSize = src->componentLayouts[type].size;
					// 获取源数据指针
					uint8_t* srcData = srcChunk->GetComponentArray(type) + compSize * srcIndex;
					// 获取目标数据指针
					uint8_t* destData = destChunk->GetComponentArray(type) + compSize * destIndex;

					// 复制数据
					src->componentLayouts[type].copyFunc(destData, srcData);
				}
			}
		}

		template<typename... Components>
		ZVector<EntityID> Query()
		{
			ZVector<EntityID> outEntities;
			ZVector<ComponentID> componentTypes = GetComponentTypes<Components...>();

			std::sort(componentTypes.begin(), componentTypes.end());

			for (auto& [_, archetype] : archetypes)
			{
				bool match = true;
				for (auto componentType : componentTypes)
				{
					if (archetype.componentLayouts.find(componentType) == archetype.componentLayouts.end())
					{
						match = false;
						break;
					}
				}

				if (match)
				{
					ZVector<EntityID> archetypeEntities = archetype.GetEntities();

					for (auto id : archetypeEntities)
					{
						outEntities.push_back(id);
					}
					
				}
			}

			return outEntities;
		}


	private:
		std::unordered_map<EntityID, ComponentTuple> entities;
		std::unordered_map<ArchetypeID, Archetype> archetypes;
		//ArchetypeID = size_t, just ID
	};


}

#endif // !ARCHETYPE_WORLD_H