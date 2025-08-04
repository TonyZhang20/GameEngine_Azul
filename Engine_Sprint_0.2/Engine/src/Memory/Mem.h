//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef MEM_H
#define MEM_H

namespace Azul
{
	//-------------------------------------------------------------------------
	// Namespace Weird thing
	//   Please declare your forward declarations INSIDE namespace
	//   Outside the namespace cause all kinds of hell
	//-------------------------------------------------------------------------

	// forward declaration
	class Heap;
	class HeapFixed;
	class HeapNormal;
	class Block;
	class Registration;

	// Singleton
	class Mem
	{
	public:

		// Constants for the library
		static const uint32_t MAJOR_VERSION = 8;
		static const uint32_t MINOR_VERSION = 0;

		// alignment 
		enum class Align : uint32_t
		{
			Byte_4,
			Byte_8,
			Byte_16,
			Byte_32,
			Byte_64,
			Byte_128,
			Byte_256,
			Default = Byte_4
		};

		// Return codes
		enum class Code : uint32_t
		{
			OK,
			ERROR_Heap_Create,
			ERROR_Illegal_Heap,
			ERROR_Mem_Not_Initialized,
			ERROR_Invalid_Addr,
		};

		// info structure
		struct Info
		{
			size_t CurrAllocIndex;
			size_t PeakHeapCount;
			size_t CurrHeapCount;
			size_t PeakNumAlloc;
			size_t CurrNumAlloc;
			size_t PeakBytesUsed;
			size_t CurrBytesUsed;
		};

	public:
		// Public Interface ---------------- (for the customers)


		// Initialize the mem system
		static Code Create();
		static Code Destroy();

		// Create an Heap from the heap system
		static Code NormalHeap(HeapNormal *&newHeap, unsigned int heapSize, const char *const Name);
		static Code FixedHeap(HeapFixed *&newHeap, unsigned int numBlocks, int sizePerBlock, const char *const Name);

		// Create an Heap from the heap system
		static Code RemoveHeap(Heap *inHeap);

		// Information
		static Code GetInfo(Info &info);
		static Code GetHeapByAddr(Heap *&pHeap, void *p);
		static Block* DebugGetGlobalTrackingHead(void);

		// -----------------------------------------------------------------------------------
		// Add extra data or methods below this line
		// -----------------------------------------------------------------------------------
		static void AddInfo(size_t size);
		static void DecreaseInfo(size_t size);

		static void RegisterPointer(void* p);
		static void UnRegisterPointer(void* p);
		static bool FindPointer(void* p);

		static void AddBlock(Block* b);
		static void RemoveBlock(Block* b);

		static HeapNormal* ReAlloc(HeapNormal* heap, size_t targetSize);

	protected:
		Mem();
		Mem(const Mem &in) = delete;
		Mem &operator = (const Mem &in) = delete;
		Mem(Mem &&) = delete;
		Mem &operator = (Mem &&) = delete;
		~Mem();

	protected:
		// Data:
		static Mem *psMem; // cannot use meyers singleton
		// multi threaded issues

		Heap *poHead;
		Block *pGlobalHead;
		Registration *poRegistration;  // pointer registration
		Info	      mInfo;
		bool	      mInitialized;
		char		 pad[ 7 ];  // x64 padding

	private:
		static void AddHeapNum();
		static int Max(int left, int right);
	};

}

#endif 

// --- End of File ---
