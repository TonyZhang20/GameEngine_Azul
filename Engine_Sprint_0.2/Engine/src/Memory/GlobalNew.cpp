//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------
#include <crtdbg.h>
#include "Mem.h"
#include "Block.h"
#include "GlobalNew.h"

//-------------------------------------------------------------------------
// Namespace Weird thing:
//   Don't use namespace here...
//   Since new/delete isn't allowed namespace
//   do all declaraton with full name Azul::Heap... or Azul::Mem::Align...
//-------------------------------------------------------------------------

// --------------------------------------------
// For Normal Heap
// --------------------------------------------

void *operator new( size_t inSize, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum )
{
	//AZUL_REPLACE_ME(inSize);
	//AZUL_REPLACE_ME(pHeap);
	//AZUL_REPLACE_ME(align);
	//AZUL_REPLACE_ME(inName);
	//AZUL_REPLACE_ME(lineNum);

	assert(pHeap != nullptr);

	Azul::Mem::Create();

	size_t alignment = 4 <<  static_cast<int>(align);
	//void* p = malloc(inSize);
	//pHeap->AddInfo(inSize);

	void* p = pHeap->Alloc(inSize, alignment, inName, lineNum);
	Azul::Mem::RegisterPointer(p);

	return p;
}

void operator delete( void *p, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum )
{
	// -----------------------------------------
	// You don't need to implement this... 
	// its here to shut up compiler warnings
	// just this function
	//------------------------------------------
	AZUL_REPLACE_ME(p);
	AZUL_REPLACE_ME(pHeap);
	AZUL_REPLACE_ME(align);
	AZUL_REPLACE_ME(inName);
	AZUL_REPLACE_ME(lineNum);
}

void *operator new[](size_t inSize, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum)
{

	Azul::Mem::Create();

	assert(pHeap != nullptr);

	size_t alignment = 4 << static_cast<int>(align);

	void* p = pHeap->Alloc(inSize, alignment, inName, lineNum);
	Azul::Mem::RegisterPointer(p);

	return p;
}

void operator delete[](void *p, Azul::HeapNormal *const pHeap, Azul::Mem::Align align, const char *inName, size_t lineNum)
{
	// -----------------------------------------
	// You don't need to implement this... 
	// its here to shut up compiler warnings
	// just this function
	//------------------------------------------
	AZUL_REPLACE_ME(p);
	AZUL_REPLACE_ME(pHeap);
	AZUL_REPLACE_ME(align);
	AZUL_REPLACE_ME(inName);
	AZUL_REPLACE_ME(lineNum);
}

// --------------------------------------------
// For Fixed Heap
// --------------------------------------------

void *operator new( size_t inSize, Azul::HeapFixed *const pHeap, const char *inName, size_t lineNum )
{
	Azul::Mem::Create();

	assert(pHeap != nullptr);

	size_t alignment = 4 << static_cast<int>(Azul::Mem::Align::Default);

	void* p = pHeap->Alloc(inSize, alignment, inName, lineNum);

	Azul::Mem::RegisterPointer(p);

	return p;
}

void operator delete( void *p, Azul::HeapFixed *const pHeap, const char *inName, size_t lineNum )
{
	// -----------------------------------------
	// You don't need to implement this... 
	// its here to shut up compiler warnings
	// just this function
	//------------------------------------------
	AZUL_REPLACE_ME(p);
	AZUL_REPLACE_ME(pHeap);
	AZUL_REPLACE_ME(inName);
	AZUL_REPLACE_ME(lineNum);
}

// --------------------------------------------
// For Global Standard new/delete
// --------------------------------------------

void *operator new( size_t inSize )
{
	void* p = _malloc_dbg(inSize, _NORMAL_BLOCK, __FILE__, __LINE__);
	return p;
}

void operator delete( void *p )
{
	if (Azul::Mem::FindPointer(p))
	{
		//Find the block
		//block find the heap
		//heap.Free
		//Unregister
		Azul::Mem::UnRegisterPointer(p);
		
		char* pointer = (char*)p;

		Azul::Block* block = *((Azul::Block**)(pointer - sizeof(void*)));
		
		Azul::Heap* heap = block->GetHeap();

		heap->Free(block);
	}
	else
	{
		_free_dbg(p, _NORMAL_BLOCK);
	}
}

void *operator new[](size_t inSize)
{
	void *p = malloc(inSize);
	return p;
}

void operator delete[](void *p)
{
	if (Azul::Mem::FindPointer(p))
	{
		//Find the block
		//block find the heap
		//heap.Free
		//Unregister
		Azul::Mem::UnRegisterPointer(p);
		
		Azul::Block* block = *((Azul::Block**)((char*)p - sizeof(void*)));
		
		Azul::Heap* heap = block->GetHeap();

		heap->Free(block);
	}
	else
	{
		free(p);
	}
}


// --- End of File ---
