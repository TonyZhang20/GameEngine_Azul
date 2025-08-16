#ifndef CHUNK_H
#define CHUNK_H

#include <unordered_map>
#include "Utility.h"
#include "Entity.h"

namespace zecs
{
	class Archetype;

	class Chunk
	{
	public:
		static constexpr size_t kSize = 8192; // 1024 * 8 == 8 kb

		explicit Chunk(Archetype* archetype);
		~Chunk();

		Chunk(const Chunk&) = delete;
		Chunk& operator=(const Chunk&) = delete;

		EntityID* GetEntityArray() const;
		// 获取组件数组指针
		template <typename T>
		T* GetComponentArray() const;

		uint8_t* GetComponentArray(ComponentID cid) const;

		bool HasSpace() const;

		template <typename... Components>
		size_t AddEntity(EntityID entity, Components &&...components);
		size_t AllocateEntity(EntityID entity);

		template <typename Component>
		void CopyComponentData(size_t index, Component&& component);
		EntityID RemoveEntity(size_t index);

		inline size_t GetEntityCount() const { return entityCount; }

	private:
		// 已知类型，逐个拷贝组件的数据
		template <typename T, typename... Remains>
		void CopyComponentDataRecursive(size_t index, T&& component, Remains &&...remains);

		inline void CopyComponentDataRecursive(size_t index) {}

	private:
		uint8_t* buffer = nullptr;      // 内存块指针
		Archetype* archetype = nullptr; // 所属 Archetype
		size_t entityCount;             // 当前实体数量
		size_t capacity;                // 最大实体容量

	};
}

#include "Chunk.inl"

#endif // !CHUNK_H
