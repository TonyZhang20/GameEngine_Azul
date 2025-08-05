//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "Mem.h"
#include "Heap.h"
#include "Block.h"
#include "HeapFixed.h"
#include "Registration.h"
#include "GlobalNew.h"

namespace Azul
{

	Mem *Mem::psMem = nullptr;

	Mem::~Mem()
	{
		//free(poRegistration);
		auto deleteMe = poRegistration;
		this->poRegistration = nullptr;

		delete deleteMe;
	}

	Mem::Mem()
	{
		this->mInfo.CurrAllocIndex = 0;
		this->mInfo.PeakHeapCount = 0;
		this->mInfo.CurrHeapCount = 0;
		this->mInfo.PeakNumAlloc = 0;
		this->mInfo.CurrNumAlloc = 0;
		this->mInfo.PeakBytesUsed = 0;
		this->mInfo.CurrBytesUsed = 0;

		this->poHead = nullptr;
		this->poRegistration = new Registration();
	}

	// Create the heap.
	Mem::Code Mem::NormalHeap(HeapNormal*& newHeap, unsigned int heapSize, const char* const Name)
	{
		if (psMem == nullptr)
		{
			newHeap = nullptr;
			return Code::ERROR_Mem_Not_Initialized;
		}

		HANDLE h = ::HeapCreate(0, heapSize, 0);
		void* s = HeapAlloc(h, 0, sizeof(Heap));

		newHeap = (HeapNormal*) new(s) Heap(h, heapSize, Name, Heap::Type::NORMAL, psMem, s);

		psMem->poRegistration->RegisterPointer(newHeap);

		Heap* heap = (Heap*)newHeap;

		if (psMem->poHead)
		{
			DLink* node = (DLink*)psMem->poHead;

			//inNode -> next = head; 
			heap->hLink.pNext = node;

			//head->prev = inNode
			psMem->poHead->hLink.pPrev = (DLink*)heap;
		}

		psMem->poHead = heap;

		AddHeapNum();

		return Code::OK;
	}

	Mem::Code Mem::FixedHeap(HeapFixed *&newHeap, unsigned int numBlocks, int sizePerBlock, const char *const Name)
	{
		if (psMem == nullptr)
		{
			newHeap = nullptr;
			return Code::ERROR_Mem_Not_Initialized;
		}

		HANDLE h = ::HeapCreate(0, numBlocks * sizePerBlock, 0);
		void* s = ::HeapAlloc(h, 0, sizeof(Heap));

		newHeap = new(s) HeapFixed(h, numBlocks * sizePerBlock, Name, Heap::Type::FIXED, psMem);
		newHeap->CreateFixedBlock(numBlocks, sizePerBlock);

		psMem->poRegistration->RegisterPointer(newHeap);

		Heap* heap = (Heap*)newHeap;

		if (psMem->poHead)
		{
			DLink* node = (DLink*)psMem->poHead;

			heap->hLink.pNext = node;
			psMem->poHead->hLink.pPrev = (DLink*)heap;
		}

		psMem->poHead = heap;

		AddHeapNum();

		return Code::OK;
	}

	// Initialize the memory system  (Public method)
	Mem::Code Mem::Create(void)
	{
		if (psMem == nullptr)
		{
			psMem = new Mem();
		}

		psMem->mInitialized = true;
		
		return Code::OK;
	}

	Mem::Code Mem::Destroy(void)
	{
		if (psMem == nullptr)
		{
			return Code::ERROR_Mem_Not_Initialized;
		}

		psMem->mInitialized = false;
		psMem->poHead = nullptr;

		Heap* node = psMem->poHead;

		while (node != nullptr)
		{
			node->CallDestructForAllBlock();

			HeapDestroy(node->mWinHeapHandle);
			node = node->GetNext();
		}
		//psMem->poRegistration->Clear();

		delete psMem;
		psMem = nullptr;

		return Code::OK;
	}

	Mem::Code Mem::GetInfo(Mem::Info& retInfo)
	{
		if (psMem == nullptr)
		{
			return Code::ERROR_Mem_Not_Initialized;
		}

		retInfo = psMem->mInfo;

		return Code::OK;
	}

	Block* Mem::DebugGetGlobalTrackingHead()
	{
		return psMem->pGlobalHead;
	}

	Mem::Code Mem::RemoveHeap(Heap *pInHeap)
	{
		if (psMem == nullptr)
		{
			return Code::ERROR_Mem_Not_Initialized;
		}

		if (pInHeap == nullptr || ((uintptr_t)pInHeap % 4) != 0)
		{
			return Code::ERROR_Illegal_Heap;
		}

		if (psMem->poHead)
		{
			Heap* node = psMem->poHead;
			Heap* prev = nullptr;
			Heap* next = nullptr;

			while (node)
			{
				if (node == pInHeap)
				{
					prev = node->GetPrev();
					next = node->GetNext();

					if (prev)
					{
						prev->SetNext(node->GetNext());
					}

					if (next)
					{
						next->SetPrev(prev);
					}

					if (node == psMem->poHead)
					{
						psMem->poHead = next;
					}

					psMem->mInfo.CurrHeapCount--;

					psMem->mInfo.CurrBytesUsed -= node->mInfo.CurrBytesUsed;
					psMem->mInfo.CurrNumAlloc -= node->mInfo.CurrNumAlloc;

					//Do delete thing
					//Call Heap Block Destruct
					node->CallDestructForAllBlock();


					return Code::OK;
				}

				node = node->GetNext();
			}

			//Doesnt find
			return Code::ERROR_Illegal_Heap;

		}
		else
		{
			return Code::ERROR_Illegal_Heap;
		}

	}

	Mem::Code Mem::GetHeapByAddr(Heap *&pHeap, void *p)
	{
		if (psMem == nullptr)
		{
			return Code::ERROR_Mem_Not_Initialized;
		}

		if (p == nullptr)
		{
			return Code::ERROR_Invalid_Addr;
		}

		char* pSecret = (char*)p - sizeof(void*);

		if (!pSecret || !psMem->poRegistration->FindPointer(p))
		{
			return Code::ERROR_Invalid_Addr;
		}

		Block* b = nullptr;
		b = *((Block**)(pSecret));

		if (!b) return Code::ERROR_Invalid_Addr;

		Heap* result = b->GetHeap();
		if (!result) return Code::ERROR_Invalid_Addr;

		pHeap = result;

		return Code::OK;
	}


	void Mem::AddHeapNum()
	{
		psMem->mInfo.CurrHeapCount++;
		psMem->mInfo.PeakHeapCount = Max(psMem->mInfo.CurrHeapCount, psMem->mInfo.PeakHeapCount);
	}

	int Mem::Max(int left, int right)
	{
		return  left > right ? left : right;
	}

	void Mem::AddInfo(size_t size)
	{
		psMem->mInfo.CurrAllocIndex++;

		psMem->mInfo.CurrBytesUsed += size;
		psMem->mInfo.PeakBytesUsed = Max(psMem->mInfo.PeakBytesUsed, psMem->mInfo.CurrBytesUsed);

		psMem->mInfo.CurrNumAlloc++;
		psMem->mInfo.PeakNumAlloc = Max(psMem->mInfo.PeakNumAlloc, psMem->mInfo.CurrNumAlloc);
	}

	void Mem::DecreaseInfo(size_t size)
	{
		if (!psMem) return;

		psMem->mInfo.CurrBytesUsed -= size;
		psMem->mInfo.CurrNumAlloc--;
	}

	void Mem::RegisterPointer(void* p)
	{
		psMem->poRegistration->RegisterPointer(p);
	}

	void Mem::UnRegisterPointer(void* p)
	{
		psMem->poRegistration->UnRegister(p);
	}

	bool Mem::FindPointer(void* p)
	{
		if (!psMem || !psMem->poRegistration) return false;
		return psMem->poRegistration->FindPointer(p);
	}

	void Mem::AddBlock(Block* b)
	{
		if (psMem->pGlobalHead != nullptr)
		{
			b->SetGlobalNext(psMem->pGlobalHead);
			psMem->pGlobalHead->SetGlobalPrev(b);

			b->SetAllocIndex(psMem->pGlobalHead->GetAllocIndex() + 1);
		}
		else
		{
			b->SetAllocIndex(1);
		}

		psMem->pGlobalHead = b;
	}

	void Mem::RemoveBlock(Block* b)
	{
		if (!psMem) return;

		if (psMem->pGlobalHead == b)
		{
			psMem->pGlobalHead = b->GetGlobalNext();

			if(psMem->pGlobalHead)
				psMem->pGlobalHead->SetGlobalPrev(nullptr);
		}
		else
		{
			Block* prev = b->GetGlobalPrev();
			Block* next = b->GetGlobalNext();

			if (prev)
			{
				prev->SetGlobalNext(next);
			}

			if (next)
			{
				next->SetGlobalPrev(prev);
			}
		}

		psMem->UnRegisterPointer(b);

		b->ClearGlobalLink();
	}

	HeapNormal* Mem::ReAlloc(HeapNormal* heap, size_t targetSize)
	{
		HANDLE hd = heap->mWinHeapHandle;

		void* originalPtr = heap->GetStartPointer();  // 从 heap 获取旧内存指针
		void* newPtr = HeapReAlloc(hd, HEAP_REALLOC_IN_PLACE_ONLY, originalPtr, targetSize);

		if (!newPtr)
			return nullptr;

		return (HeapNormal*)heap;
	}


}

// --- End of File ---
