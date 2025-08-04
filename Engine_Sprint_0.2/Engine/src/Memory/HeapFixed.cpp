//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "HeapFixed.h"
#include "Block.h"
#include "GlobalNew.h"

namespace Azul
{
	void HeapFixed::SetOverflowHeap(Heap *pHeap)
	{
		this->pOverflow = pHeap;
	}

	Heap *HeapFixed::GetOverflowHeap() const
	{
		return this->pOverflow;
	}

	void* HeapFixed::Alloc(size_t insize, size_t align, const char* inName, size_t lineNum)
	{
		void* userBlock;
		Block* b = nullptr;

		if (this->pFixedFreeHead)
		{
			b = PopFreeBlock();
			this->pMem->AddInfo(insize);

			this->AddInfo(insize);
			this->AddBlock(b);

			userBlock = b->pUserBlock;

			b->SetData(inName, lineNum);
		}
		else			//Heap Over Flow
		{
			userBlock = this->HeapOverFlow(insize, inName, lineNum);
		}

		char* pointer = (char*)userBlock;

		return userBlock;
	}

	void HeapFixed::Free(Block* p)
	{
		this->PushFreeBlock(p);

		this->RemoveBlock(p);

		this->DecreaseInfo(p->mAllocSize);
		this->pMem->DecreaseInfo(p->mAllocSize);
	}

	void HeapFixed::CreateFixedBlock(unsigned int numBlocks, int sizePerBlock)
	{
		//For pAligned allignment
		size_t requireSize = sizeof(Block) * numBlocks + sizePerBlock * numBlocks + 3;
		void* pRaw = HeapAlloc(this->mWinHeapHandle, 0, requireSize);
		char* charRow = (char*)pRaw;

		char* pAligned = (char*)((uintptr_t)(charRow + (3)) & ~(3));

		size_t stepSize = sizeof(Block) + sizePerBlock;

		//Botton
		pAligned = pAligned + numBlocks * (sizePerBlock + sizeof(Block));

		for (int i = 0; i < numBlocks; i++)
		{
			pAligned -= (sizeof(Block) + (size_t)sizePerBlock);

			Block* block = new(pAligned) Block();

			char* blocksize = (char*)block;

			this->PushFreeBlock(block);

			//End of the block
			char* userBlock = pAligned + sizeof(Block);

			//secret
			char* pSecret = userBlock - sizeof(void*);
			*((void**)pSecret) = pAligned;

			block->pHeap = this;
			block->pSecret = pAligned;
			block->pUserBlock = userBlock;
			block->mAllocSize = sizePerBlock;
		}

	}

	void* HeapFixed::HeapOverFlow(size_t insize, const char* inName, size_t lineNum)
	{
		assert(this->pOverflow != nullptr);
		
		size_t alignment = 4 << static_cast<int>(Azul::Mem::Align::Default);

		void * p = this->pOverflow->Alloc(insize, alignment, inName, lineNum);
		Azul::Mem::RegisterPointer(p);

		return p;
	}

	HeapFixed::HeapFixed(HANDLE winHeapHandle, unsigned int HeapSize, const char* const Name, Type hType, Mem* pMem)
		: Heap(winHeapHandle, HeapSize, Name, hType, pMem)
	{

	}

	void HeapFixed::PushFreeBlock(Block* b)
	{
		if (this->pFixedFreeHead != nullptr)
		{
			b->SetFreeNext(pFixedFreeHead);
		}

		this->pFixedFreeHead = b;
	}

	Block* HeapFixed::PopFreeBlock()
	{
		Block* result = this->pFixedFreeHead;

		this->pFixedFreeHead = this->pFixedFreeHead->GetFreeNext();

		assert(result != nullptr);

		result->SetFreeNext(nullptr);

		return result;
	}

}

// --- End of File ---
