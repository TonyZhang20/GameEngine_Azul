//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef BLOCK_H
#define BLOCK_H

#include "DLink.h"

namespace Azul
{
	class Heap;  // forward declare

	//-------------------------------------------------------------------------
	// Weird thing
	//   Please declare your forward declarations INSIDE namespace
	//   Outside the namespace cause all kinds of hell
	//-------------------------------------------------------------------------

	class Block
	{
	public:
		// defines for the heap name length
		static const uint32_t NameSize = 64;
		friend class Heap;
		friend class HeapFixed;

	public:
		// Public Interface ---------------- (for the customers)

		Block *GetHeapNext() const;
		Block *GetHeapPrev() const;

		void SetHeapNext(Block*);
		void SetHeapPrev(Block*);

		Block* GetFreeNext() const;
		Block* GetFreePrev() const;

		void SetFreeNext(Block*);
		void SetFreePrev(Block*);

		Block *GetGlobalNext() const;
		Block *GetGlobalPrev() const;

		void SetGlobalNext(Block*);
		void SetGlobalPrev(Block*);

		const char *GetFileName() const;

		void SetAllocIndex(size_t index);

		size_t GetLineNum() const;
		size_t GetAllocIndex() const;
		size_t GetAllocSize() const;

		Heap *GetHeap() const;
		void *GetUserBlock() const;

		void ClearGlobalLink();
		void ClearHeapLink();

		void SetData(const char* inName, size_t lineNum);
		
	protected:
		// Selective Access Pattern
		Block(const char *inName, size_t lineNum);
		Block();

	protected:
		Block(const Block &) = delete;
		Block &operator = (const Block &) = delete;
		Block(Block &&) = delete;
		Block &operator = (Block &&) = delete;
		~Block() = default;

	private:
		// Add tracking links 
		DLink bLink;   // Block links
		DLink gLink;   // Global links
		DLink fLink;   // Free Links

		// stats
		Heap *pHeap;
		void *pUserBlock;

		size_t	mLineNum;
		size_t	mAllocIndex;
		size_t	mAllocSize;

		char	mFileName[ Block::NameSize ];

		// back pointer
		void *pSecret;   // back pointer to the top of the trackingblock
	};

}

#endif

// ---  End of File ---
