#include "Chunk.h"
#include "Archetype.h"

namespace zecs
{
    inline Archetype::Archetype(size_t hash, std::unordered_map<ComponentID, ComponentLayout>& layouts)
        : hash(hash), componentLayouts(layouts)
    {
        // 这里可以初始化 chunkCapacity 等
        CalculateLayout();

        for (auto& [type, _] : layouts)
        {
            componentTypes.push_back(type);
        }
    }

    inline Archetype::~Archetype()
    {
        for (size_t i = 0; i < chunks.size(); --i)
        {
            if (chunks[i])
            {
                delete chunks[i];
                chunks[i] = nullptr;
            }
        }
    }

    template<typename... Components>
    std::tuple<Chunk*, size_t> Archetype::AddEntity(EntityID entity, Components&&... components)
    {
        // 找到一个有空位的 Chunk，否则创建新的
        Chunk* targetChunk = nullptr;

        for (auto& chunk : this->chunks)
        {
            if (chunk->HasSpace())
            {
                targetChunk = chunk;
                break;
            }
        }

        if (!targetChunk)
        {
            targetChunk = new Chunk(this);
            chunks.push_back(targetChunk);
        }

        size_t idx = targetChunk->AddEntity(entity, std::forward<Components>(components)...);
        return { targetChunk, idx };
    }

    inline std::tuple<Chunk*, size_t> Archetype::AllocateEntity(EntityID entity)
    {
        // 找到一个有空位的 Chunk，否则创建新的
        Chunk* targetChunk = nullptr;

        for (auto& chunk : this->chunks)
        {
            if (chunk->HasSpace())
            {
                targetChunk = chunk;
                break;
            }
        }

        if (!targetChunk)
        {
            targetChunk = new Chunk(this);
            chunks.push_back(targetChunk);
        }

        size_t idx = targetChunk->AddEntity(entity);
        return { targetChunk, idx };
    }

    inline ZVector<EntityID> Archetype::GetEntities() const
    {
        ZVector<EntityID> outEntities;

        for (auto& chunk : chunks)
        {
            EntityID* entityArray = chunk->GetEntityArray();

            for (int i = 0; i < chunk->GetEntityCount(); i++)
            {
                outEntities.push_back(entityArray[i]);
            }
        }

        return outEntities;
    }

    inline void Archetype::CalculateLayout()
    {
        constexpr size_t kChunkSize = Chunk::kSize; // 16KB

        // 估算单个实体所需空间
        size_t perEntitySize = sizeof(EntityID);
        for (const auto& [_, layout] : componentLayouts)
        {
            perEntitySize += layout.size;
        }
        // 估算初始容量
        size_t estimatedCapacity = std::max((size_t)1, kChunkSize / perEntitySize);
        size_t finalCapacity = 0;

        // 迭代计算实际容量
        for (int i = 0; i < 5; i++)
        {
            size_t currentOffset = 0;
            size_t requiredSpace = 0;

            // 实体数组所需容量
            size_t entityArraySize = sizeof(EntityID) * estimatedCapacity;
            requiredSpace = currentOffset + entityArraySize;
            currentOffset = requiredSpace;

            // 组件数组所需容量
            for (const auto& [_, layout] : componentLayouts)
            {
                currentOffset = AlignUp(currentOffset, layout.alignment);
                size_t compArraySize = layout.size * estimatedCapacity;
                requiredSpace = currentOffset + compArraySize;
                currentOffset = requiredSpace;
            }

            // 检查是否满足空间
            if (requiredSpace <= kChunkSize)
            {
                finalCapacity = estimatedCapacity;
                estimatedCapacity++; // 满足则尝试更大的容量
            }
            else
            {
                break; // 超过空间，使用上次成功的容量
            }
        }

        // 确保至少容纳1个实体
        chunkCapacity = std::max((size_t)1, finalCapacity);

        // 确定好总容量后，开始计算各数组的偏移量
        size_t currentOffset = 0;

        // 实体数组偏移为0
        currentOffset += sizeof(EntityID) * chunkCapacity;

        // 组件数组偏移
        for (auto& [_, layout] : componentLayouts)
        {
            currentOffset = AlignUp(currentOffset, layout.alignment);
            layout.offset = currentOffset;
            currentOffset += layout.size * chunkCapacity;
        }
    }
}