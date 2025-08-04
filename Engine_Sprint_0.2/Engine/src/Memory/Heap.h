//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef HEAP_H
#define HEAP_H

#include "Mem.h"
#include "DLink.h"

namespace  Azul
{
	//-------------------------------------------------------------------------
	// Namespace Weird thing
	//   Please declare your forward declarations INSIDE namespace
	//   Outside the namespace cause all kinds of hell
	//-------------------------------------------------------------------------

	// Forward declaration
	class Block;

	// Heap class
	class Heap
	{
	public:
		// Heap Types
		enum class Type : size_t
		{
			NORMAL,
			FIXED
		};

		// Info structure
		struct Info
		{
			// Constants 
			static const uint32_t NameSize = 64;

			// Data
			size_t    TotalSize;
			size_t	  PeakNumAlloc;
			size_t	  CurrNumAlloc;
			size_t	  PeakBytesUsed;
			size_t	  CurrBytesUsed;
			char Name[ Heap::Info::NameSize ];
		};

	public:
		// Public Interface ---------------- (for the customers)

		void GetInfo(Info &info) const;

		// Tracking block 
		Block *GetTrackingHead() const;
		Heap::Type GetHeapType() const;

		bool HasEnoughSize(size_t userSize, size_t alignment = 4);

		void AddInfo(size_t size);
		void DecreaseInfo(size_t size);

		virtual void* Alloc(size_t insize, size_t align, const char* inName, size_t lineNum);
		virtual void Free(Block* p);

		const char* GetName() const;

		void* GetStartPointer();
		void CallDestructForAllBlock();

	protected:
		void SetPrev(Heap* pInheap);
		void SetNext(Heap* pInheap);
		Heap* GetNext() const;
		Heap* GetPrev() const;
		bool hasNext();
		bool hasPrev();

		friend class Mem;

	protected:
		Heap(HANDLE winHeapHandle, unsigned int HeapSize, const char *const Name, Type hType, Mem *pMem, void* originalptr = nullptr);

	protected:
		Heap() = delete;
		Heap(const Heap &) = delete;
		Heap &operator = (const Heap &tmp) = delete;
		Heap(Heap &&) = delete;
		Heap &operator = (Heap &&) = delete;
		virtual ~Heap() = default;

	protected:
		// Data:
		Block *pBlkHead;
		Mem *pMem;     // Back link to the memory system
		DLink   hLink;    // Heap Link
		Type	mType;
		HANDLE  mWinHeapHandle;
		void* originalPtr;
		Info	mInfo;
		virtual void AddBlock(Block* b);
		virtual void RemoveBlock(Block* b);

	private:
		int Max(int left, int right);
	};

}

#endif

// --- End of File ---
