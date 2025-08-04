//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef HEAP_NORMAL_H
#define HEAP_NORMAL_H

#include "Heap.h"


namespace Azul
{
	//-------------------------------------------------------------------------
	// Namespace Weird thing
	//   Please declare your forward declarations INSIDE namespace
	//   Outside the namespace cause all kinds of hell
	//-------------------------------------------------------------------------

	// Forward declaration

	// Heap class
	class HeapNormal : public Heap
	{
	public:
		// Public Interface ---------------- (for the customers)

	protected:
		HeapNormal(HANDLE winHeapHandle, unsigned int HeapSize, const char *const Name, Type hType, Mem *pMem);

	protected:
		HeapNormal() = delete;
		HeapNormal(const HeapNormal &) = delete;
		HeapNormal &operator = (const HeapNormal &tmp) = delete;
		HeapNormal(HeapNormal &&) = delete;
		HeapNormal &operator = (HeapNormal &&) = delete;
		virtual ~HeapNormal() = default;

	private:
		// Data:
	};

}

#endif

// --- End of File ---
