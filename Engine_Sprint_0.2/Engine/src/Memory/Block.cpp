//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "Block.h"

namespace Azul
{
	Block::Block(const char* inName, size_t lineNum)
	{

		const char* lastSlash = strrchr(inName, '\\');
		const char* fileName = lastSlash + 1;

		size_t len = strlen(fileName);

		if (len > Block::NameSize)
		{
			len = Block::NameSize;
		}

		memset(this->mFileName, 0x0, Block::NameSize);
		memcpy(this->mFileName, fileName, len);

		this->mLineNum = lineNum;

		ClearGlobalLink();
		ClearHeapLink();

		this->pHeap = nullptr;
		this->pUserBlock = nullptr;
		this->pSecret = nullptr;
	}

	Block::Block()
	{
		ClearGlobalLink();
		ClearHeapLink();

		this->pHeap = nullptr;
		this->pUserBlock = nullptr;
		this->pSecret = nullptr;
	}

	void *Block::GetUserBlock() const
	{
		return pUserBlock;
	}

	void Block::ClearGlobalLink()
	{
		this->gLink.pNext = nullptr;
		this->gLink.pPrev = nullptr;
	}

	void Block::ClearHeapLink()
	{
		this->bLink.pNext = nullptr;
		this->bLink.pPrev = nullptr;
	}

	void Block::SetData(const char* inName, size_t lineNum)
	{
		const char* lastSlash = strrchr(inName, '\\');
		const char* fileName = lastSlash + 1;

		size_t len = strlen(fileName);

		if (len > Block::NameSize)
		{
			len = Block::NameSize;
		}

		memset(this->mFileName, 0x0, Block::NameSize);
		memcpy(this->mFileName, fileName, len);

		this->mLineNum = lineNum;
	}

	Block *Block::GetHeapNext() const
	{
		return (Block*)bLink.pNext;
	}

	Block *Block::GetHeapPrev() const
	{
		return (Block*)bLink.pPrev;
	}

	void Block::SetHeapNext(Block* b) 
	{
		this->bLink.pNext = (DLink*)b;
	}

	void Block::SetHeapPrev(Block* b)
	{
		assert(this);
		
		this->bLink.pPrev = (DLink*)b;
	}

	Block* Block::GetFreeNext() const
	{
		return (Block*)fLink.pNext;
	}

	Block* Block::GetFreePrev() const
	{
		return (Block*)bLink.pPrev;
	}

	void Block::SetFreeNext(Block* b)
	{
		this->fLink.pNext = (DLink*)b;
	}

	void Block::SetFreePrev(Block* b)
	{
		this->fLink.pPrev = (DLink*)b;
	}

	Block *Block::GetGlobalNext() const
	{
		return (Block*)gLink.pNext;
	}

	Block *Block::GetGlobalPrev() const
	{
		return (Block *)gLink.pPrev;
	}

	void Block::SetGlobalNext(Block* b)
	{
		this->gLink.pNext = (DLink*)b;
	}

	void Block::SetGlobalPrev(Block* b)
	{
		this->gLink.pPrev = (DLink*)b;
	}

	const char *Block::GetFileName() const
	{
		return this->mFileName;
	}

	void Block::SetAllocIndex(size_t index)
	{
		this->mAllocIndex = index;
	}

	Heap *Block::GetHeap() const
	{
		return this->pHeap;
	}

	size_t Block::GetLineNum() const
	{
		return this->mLineNum;
	}

	size_t Block::GetAllocIndex() const
	{
		return this->mAllocIndex;
	}

	size_t Block::GetAllocSize() const
	{
		return this->mAllocSize;
	}

}

// --- End of File ---
