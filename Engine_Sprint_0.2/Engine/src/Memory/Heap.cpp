//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "Heap.h"
#include "Block.h"
#include "GlobalNew.h"

namespace Azul
{

	Heap::Type Heap::GetHeapType() const
	{
		return this->mType;
	}

	bool Heap::HasEnoughSize(size_t userSize, size_t alignment)
	{
		size_t alignedSize = (userSize + alignment - 1) & ~(alignment - 1);
		size_t totalRequired = alignedSize + sizeof(Block) + mInfo.CurrBytesUsed;
		return totalRequired <= mInfo.TotalSize;
	}

	void Heap::AddInfo(size_t size)
	{
		mInfo.CurrBytesUsed += size;
		mInfo.PeakBytesUsed = this->Max(mInfo.PeakBytesUsed, mInfo.CurrBytesUsed);

		mInfo.CurrNumAlloc++;
		mInfo.PeakNumAlloc = this->Max(mInfo.PeakNumAlloc, mInfo.CurrNumAlloc);
	}

	void Heap::DecreaseInfo(size_t size)
	{
		mInfo.CurrBytesUsed -= size;
		mInfo.CurrNumAlloc--;
	}

	void* Heap::Alloc(size_t insize, size_t align ,const char* inName, size_t lineNum)
	{
		size_t requireSize = sizeof(Block) + insize + align - 1;

		//TODO: Check Over Flow
		//heap.info.totalsize < used + require
		//Heap OverFlow
		void* pRaw = HeapAlloc(this->mWinHeapHandle, 0, requireSize);

		Block* block = new(pRaw) Block(inName, lineNum);

		this->AddBlock(block);

		//End of the block
		char* p = (char*)pRaw + sizeof(Block);

		*((Block**)p) = block;

		//Alignment
		char* pAligned = (char*)((uintptr_t)(p + (align - 1)) & ~(align - 1));

		char* pSecret = pAligned - sizeof(void*);

		*((Block**)pSecret) = block;

		block->pSecret = block;
		block->pUserBlock = pAligned;

		block->pHeap = this;

		//pass data
		block->mAllocSize = insize;

		this->pMem->AddInfo(insize);

		this->AddInfo(insize);

		return (void*)pAligned;
	}

	void Heap::Free(Block* block)
	{
		this->pMem->DecreaseInfo(block->mAllocSize);

		this->RemoveBlock(block);
		this->DecreaseInfo(block->mAllocSize);

		HeapFree(this->mWinHeapHandle, 0, block);
	}

	void Heap::SetPrev(Heap* pInheap)
	{
		hLink.pPrev = (DLink*)pInheap;
	}

	void Heap::SetNext(Heap* pInheap)
	{
		hLink.pNext = (DLink*)pInheap;
	}

	Heap* Heap::GetNext() const
	{
		return (Heap*)hLink.pNext;
	}

	Heap* Heap::GetPrev() const
	{
		return (Heap*)hLink.pPrev;
	}

	bool Heap::hasNext()
	{
		return hLink.pNext;
	}

	bool Heap::hasPrev()
	{
		return hLink.pPrev;
	}

	Heap::Heap(HANDLE winHeapHandle, unsigned int HeapSize, const char* const Name, Type hType, Mem* pMem, void* org)
		: mWinHeapHandle(winHeapHandle), mType(hType), pMem(pMem), originalPtr(org)
	{

		assert(Name);
		mInfo.TotalSize = HeapSize;

		size_t len = strlen(Name);

		if (len > mInfo.NameSize)
		{
			len = mInfo.NameSize;
		}

		this->pBlkHead = nullptr;

		memset(this->mInfo.Name, 0x0, this->mInfo.NameSize);
		memcpy(this->mInfo.Name, Name, len);
	}

	int Heap::Max(int left, int right)
	{
		return  left > right ? left : right;
	}

	/// <summary>
	/// Will Also call Mem add
	/// </summary>
	/// <param name="b"></param>
	void Heap::AddBlock(Block* b)
	{
		if(this->pBlkHead != nullptr)
		{
			b->SetHeapNext(pBlkHead);

			pBlkHead->SetHeapPrev(b);
		}
		
		this->pBlkHead = b;

		Azul::Mem::AddBlock(b);
	}

	/// <summary>
	/// will also call Mem.Remove()
	/// </summary>
	/// <param name="b"></param>
	void Heap::RemoveBlock(Block* b)
	{
		assert(b->GetHeap() == this);

		if (this->pBlkHead == b)
		{
			this->pBlkHead = b->GetHeapNext();

			if(this->pBlkHead)
				this->pBlkHead->SetHeapPrev(nullptr);
		}
		else
		{
			Block* prev = b->GetHeapPrev();
			Block* next = b->GetHeapNext();

			if (prev)
			{
				prev->SetHeapNext(next);
			}

			if (next)
			{
				next->SetHeapPrev(prev);
			}
		}

		b->ClearHeapLink();
		this->pMem->RemoveBlock(b);
	}

	void Heap::GetInfo(Info &retInfo) const
	{
		retInfo = this->mInfo;
	};

	Block *Heap::GetTrackingHead() const
	{
		return this->pBlkHead;
	}

	const char* Heap::GetName() const
	{
		return mInfo.Name;
	}

	void* Heap::GetStartPointer()
	{
		return originalPtr;
	}

	void Heap::CallDestructForAllBlock()
	{
		Block* node = this->pBlkHead;
		Block* next = node->GetHeapNext();

		while (node)
		{
			if (pMem)
			{
				pMem->DecreaseInfo(node->mAllocSize);
			}
			
			this->RemoveBlock(node);
			this->DecreaseInfo(node->mAllocSize);

			node = next;

			if (node)
			{
				next = node->GetHeapNext();
			}
		}

		HeapDestroy(mWinHeapHandle);
	}

}

// --- End of File ---
