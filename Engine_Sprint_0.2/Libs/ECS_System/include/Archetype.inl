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
        constexpr size_t kChunkSize = Chunk::kSize; // 8KB

        // 先求单个最小开销（以对齐到最大 alignment 来估算下界）
        size_t minEntitySize = sizeof(EntityID);
        size_t maxAlign = alignof(EntityID);

        for (const auto& [_, layout] : componentLayouts)
        {
            minEntitySize += layout.size;
            maxAlign = std::max(maxAlign, layout.alignment);
        }

        minEntitySize = AlignUp(minEntitySize, maxAlign);

        // 容量搜索边界
        size_t low = 1;
        size_t high = kChunkSize / minEntitySize;
        size_t best = 1;

        auto CalcRequiredSpace = [&](size_t capacity) -> size_t 
        {
            size_t offset = 0;
            // 实体数组
            offset += sizeof(EntityID) * capacity;
            // 组件数组
            for (const auto& [_, layout] : componentLayouts)
            {
                offset = AlignUp(offset, layout.alignment);
                offset += layout.size * capacity;
            }
            return offset;
        };

        // 二分查找最大容量
        while (low <= high)
        {
            size_t mid = (low + high) / 2;
            size_t required = CalcRequiredSpace(mid);

            if (required <= kChunkSize)
            {
                best = mid;       // 这个容量可行，尝试更大
                low = mid + 1;
            }
            else
            {
                high = mid - 1;   // 太大，缩小
            }
        }

        chunkCapacity = best;

        // ---- 最终确定偏移 ----
        size_t currentOffset = 0;

        // 实体数组偏移为0
        currentOffset += sizeof(EntityID) * chunkCapacity;

        // 组件数组偏移
        for (auto& pair : componentLayouts)
        {
            auto& layout = pair.second;

            currentOffset = AlignUp(currentOffset, layout.alignment);
            layout.offset = currentOffset;
            currentOffset += layout.size * chunkCapacity;
        }
    }

}