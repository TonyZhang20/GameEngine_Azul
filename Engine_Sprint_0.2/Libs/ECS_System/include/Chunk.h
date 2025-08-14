#include <unordered_map>

namespace zecs
{
	using ComponentID = uint32_t;

	// 通过这样的位运算来对齐内存
	size_t AlignUp(size_t offset, size_t alignment)
	{
		return (offset + alignment - 1) & ~(alignment - 1);
	}

	struct ComponentLayout
	{
		size_t offset;    // 在内存块中的偏移量  (要求)
		size_t alignment; // 内存对齐           (已知)
		size_t size;      // Component 的大小  (已知)
	};

	class Chunk
	{
		static constexpr size_t kSize = 16384; // 1024 * 16 == 16 kb


	};

	std::unordered_map<ComponentID, ComponentLayout> componentLayouts;
}