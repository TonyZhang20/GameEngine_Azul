#include "Chunk.h"
#include "ZVector.hpp"
#include <unordered_map>

namespace zecs
{
	using ComponentID = uint32_t;
	using ArchetypeID = size_t;

	class Archetype
	{
	public:
		Archetype(size_t hash, std::unordered_map<ComponentID, ComponentLayout>& layouts)
			: hash(hash), componentLayouts(layouts)
		{

		}

		static constexpr size_t CHUNK_SIZE = 16 * 1024; // 16KB
		ZVector<Chunk*> chunks;
		ArchetypeID hash;

		ZVector<ComponentID> componentTypes;
		std::unordered_map<ComponentID, ComponentLayout> componentLayouts;
		size_t chunkCapacity = 0;

	private:
	};
}