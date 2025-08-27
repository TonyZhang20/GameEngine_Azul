#ifndef Utility_H
#define Utility_H

#include <functional>

namespace zecs
{
#ifdef new
#undef new
#endif
#define ZECSASSERT(msg, expr) assert(((void)msg, expr))
#define WORLD_PAGE_SIZE 4096

	struct EntityID;

	using ArchetypeID = size_t;
	using ComponentID = uint32_t;

	class IndexGetter
	{
	public:
		template<typename T>
		static uint32_t Get()
		{
			static uint32_t id = curIdx++;
			return id;
		}

	private:
		inline static uint32_t curIdx = 0;
	};

	struct ComponentLayout
	{
		ComponentLayout() = default;

		ComponentLayout(size_t cmpSize, size_t align)
		{
			this->alignment = align;
			this->size = cmpSize;
		}

		ComponentLayout(size_t offset, size_t cmpSize, size_t align)
		{
			this->offset = offset;
			this->alignment = align;
			this->size = cmpSize;
		}

		template<typename T> 
		void RegisterLayout()
		{
			this->copyFunc = [](void* dest, const void* src) {
				new (dest) T(*reinterpret_cast<const T*>(src));
				};

			this->destructorFunc = [](void* obj) {
				reinterpret_cast<T*>(obj)->~T();
				};
		}

		size_t offset;    // 在内存块中的偏移量  (要求) -> 也就是头指针的位置 -> 通过计算得出，不需要处理
		size_t alignment; // 内存对齐           (已知) //尾部对齐
		size_t size;      // Component 的大小  (已知) //sizeof(Component)

		std::function<void(void* dest, const void* src)> copyFunc; //拷贝构造
		std::function<void(void* obj)> destructorFunc;//~T()
	};

	struct IComponentData
	{
		IComponentData() = default;
		virtual ~IComponentData() = default;
		virtual void Remove(EntityID entity) = 0;
	};

	// 通过这样的位运算来对齐内存
	static size_t AlignUp(size_t offset, size_t alignment)
	{
		return (offset + alignment - 1) & ~(alignment - 1);
	}
}


#endif // !Utility
