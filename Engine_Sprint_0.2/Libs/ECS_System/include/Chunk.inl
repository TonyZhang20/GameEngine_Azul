#include "Archetype.h"

namespace zecs
{
    inline Chunk::Chunk(Archetype* archetype)
        : archetype(archetype), entityCount(0), capacity(archetype->chunkCapacity)
    {
        buffer = static_cast<uint8_t*>(operator new(kSize, std::align_val_t{ 64 }));
        if (!buffer) throw std::bad_alloc();
    }

    inline Chunk::~Chunk()
    {
        if (buffer)
        {
            operator delete(buffer, std::align_val_t{ 64 });
        }
    }

    inline EntityID* Chunk::GetEntityArray() const
    {
        return reinterpret_cast<EntityID*>(buffer);
    }

    template <typename T>
    T* Chunk::GetComponentArray() const
    {
        uint32_t cid = IndexGetter::Get<T>();
        return reinterpret_cast<T*>(buffer + archetype->componentLayouts[cid].offset);
    }

    inline uint8_t* Chunk::GetComponentArray(ComponentID cid) const
    {
        if (archetype->componentLayouts.find(cid) == archetype->componentLayouts.end())
        {
            ZECSASSERT("Archetype does not contain component", false);
        }
        return buffer + archetype->componentLayouts[cid].offset;
    }

    inline bool Chunk::HasSpace() const
    {
        return entityCount < capacity;
    }

    inline EntityID Chunk::RemoveEntity(size_t index)
    {
        const size_t lastIndex = --entityCount;

        EntityID* entityArray = this->GetEntityArray();

        //check if last
        if (index == lastIndex || lastIndex == 0)
        {
            return EntityID{ 0xffffff, 0xff };
        }

        // swap last and index entity
        entityArray[index] = entityArray[lastIndex];

        // swap last and index entity's component
        for (auto type : archetype->componentTypes)
        {
            uint8_t* compData = buffer + archetype->componentLayouts[type].offset;
            size_t compSize = archetype->componentLayouts[type].size;

            void* src = compData + lastIndex * compSize;
            void* dest = compData + index * compSize;

            memcpy(dest, src, compSize);
        }

        // 告知更新索引
        return entityArray[lastIndex];
    }

    inline size_t Chunk::AllocateEntity(EntityID entity)
    {
        if (!this->HasSpace())
        {
            ZECSASSERT("Chunk is full", false);
        }

        const size_t index = entityCount++;

        EntityID* entities = this->GetEntityArray();
        entities[index] = entity;

        return index;
    }

    template <typename... Components>
    size_t Chunk::AddEntity(EntityID entity, Components&&... components)
    {
        if (!this->HasSpace())
        {
            ZECSASSERT("Chunk is full", false);
        }

        const size_t index = entityCount++;

        EntityID* entities = this->GetEntityArray();
        entities[index] = entity;

        this->CopyComponentDataRecursive(index, std::forward<Components>(components)...);

        return index;
    }

    template<typename Component>
    inline void Chunk::CopyComponentData(size_t index, Component&& component)
    {
        auto cid = IndexGetter::Get<Component>();

        uint8_t* compArray = buffer + archetype->componentLayouts[cid].offset;
        size_t compSize = archetype->componentLayouts[cid].size;

        Component* dest = reinterpret_cast<Component*>(compArray + index * compSize);
        *dest = std::forward<Component>(component);
    }

    template <typename T, typename... Remains>
    inline void Chunk::CopyComponentDataRecursive(size_t index, T&& component, Remains&&... remains)
    {
        auto cid = IndexGetter::Get<T>();
        uint8_t* compArray = buffer + archetype->componentLayouts[cid].offset;
        size_t compSize = archetype->componentLayouts[cid].size;

        T* dest = reinterpret_cast<T*>(compArray + index * compSize);
        *dest = std::forward<T>(component);
  
        this->CopyComponentDataRecursive(index, std::forward<Remains>(remains)...);
    }
}