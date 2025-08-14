#ifndef ENTITY_H
#define ENTITY_H

#include <functional> //for std::hash

namespace zecs
{
	//位运算优化
	struct EntityID
	{
		uint32_t index : 24; // 24 bit as ID
		uint32_t version : 8; //8 bit as version

		EntityID() : index(0), version(0) {}
		EntityID(uint32_t idx, uint32_t ver) : index(idx), version(ver) {}
		inline bool operator == (const EntityID& other) const
		{
			return index == other.index && version == other.version;
		}
	};

}

// hash 特化
namespace std
{
	template <>
	struct hash<zecs::EntityID>
	{
		size_t operator()(const zecs::EntityID& e) const noexcept
		{
			size_t h1 = std::hash<uint32_t>{}(e.index);
			size_t h2 = std::hash<uint32_t>{}(e.version);
			return h1 ^ (h2 << 1);
		}
	};
}




#endif // !ENTITY_H
