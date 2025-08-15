#ifndef ARCHETYPE_H 
#define ARCHETYPE_H

#include "Entity.h"
#include "Utility.h"
#include "ZVector.hpp"
#include <unordered_map>


namespace zecs
{
	class Chunk;

	class Archetype
	{

		//Forward declar
		friend class Chunk;
		friend class ArchetypeWorld;

	public:
		Archetype(size_t hash, std::unordered_map<ComponentID, ComponentLayout>& layouts);
		Archetype() = default;
		~Archetype();

		//传入的是已经做过分类的了
		template<typename... Components>
		std::tuple<Chunk*, size_t> AddEntity(EntityID entity, Components&&... components);
		std::tuple<Chunk*, size_t> AllocateEntity(EntityID entity);

		ZVector<EntityID> GetEntities() const;
		void CalculateLayout();

		static ArchetypeID HashComponentTypes(ZVector<ComponentID>& componentTypes)
		{
			ArchetypeID hashValue = 0;
			
			for (ComponentID id : componentTypes)
			{
				hashValue = (hashValue ^ id) * 16777619; // FNV-1a
			}

			return hashValue;
		}

	private:
		static constexpr size_t CHUNK_SIZE = 16 * 1024; // 16KB
		ZVector<Chunk*> chunks;
		ArchetypeID hash;

		ZVector<ComponentID> componentTypes;
		std::unordered_map<ComponentID, ComponentLayout> componentLayouts;
		size_t chunkCapacity = 0;
	};

}


#include "Archetype.inl" //为了处理互相引用的情况

#endif // !ARCHETYPE_H