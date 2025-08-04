//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef HEAP_FIXED_H
#define HEAP_FIXED_H

#include "Heap.h"

namespace Azul
{

	//-------------------------------------------------------------------------
	// Namespace Weird thing
	//   Please declare your forward declarations INSIDE namespace
	//   Outside the namespace cause all kinds of hell
	//-------------------------------------------------------------------------

	// Forward declaration
	class Block;

	// Heap class
	class HeapFixed : public Heap
	{
	public:

		// --------------------------------------------------------------------
		// When declaring a fixed heap... need space for overhead
		//    initial size = numAlloc*sizeBlock + sizeof(Heap) + FreeHeapExtra
		// --------------------------------------------------------------------
		static const uint32_t FreeHeapExtra = 4 * 1024;
		friend class Mem;
	public:
		// Public Interface ---------------- (for the customers)

		void SetOverflowHeap(Heap *);
		Heap *GetOverflowHeap() const;

		void* Alloc(size_t insize, size_t align, const char* inName, size_t lineNum) override;
		void Free(Block* p) override;

		void CreateFixedBlock(unsigned int numBlocks, int sizePerBlock);
		void* HeapOverFlow(size_t insize, const char* inName, size_t lineNum);

	protected:
		HeapFixed(HANDLE winHeapHandle, unsigned int HeapSize, const char *const Name, Type hType, Mem *pMem);

	protected:
		HeapFixed() = delete;
		HeapFixed(const HeapFixed &) = delete;
		HeapFixed &operator = (const HeapFixed &tmp) = delete;
		HeapFixed(HeapFixed &&) = delete;
		HeapFixed &operator = (HeapFixed &&) = delete;
		virtual ~HeapFixed() = default;

		void PushFreeBlock(Block* b);
		Block* PopFreeBlock();

	private:
		// Data: suggestion
		Block *pFixedFreeHead;
		Heap *pOverflow;

	};

}

#endif

// --- End of File ---
