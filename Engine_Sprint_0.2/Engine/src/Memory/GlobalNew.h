//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef GLOBAL_NEW_H
#define GLOBAL_NEW_H

#include "Mem.h"
#include "Heap.h"
#include "HeapFixed.h"
#include "HeapNormal.h"

//-------------------------------------------------------------------------
// Namespace Weird thing:
//    Namespaces are not allowed for new/delete
//    Also forward declaring here, causes hell
//    So include both Mem.h and Heap.h
//                  - you are welcome (you just saved 8 hrs)
//-------------------------------------------------------------------------

#ifdef new
#undef new
#endif // new


// For Normal Heap
void* operator new(size_t inSize, Azul::HeapNormal* const pHeap, Azul::Mem::Align align, const char* inName , size_t lineNum );
void operator delete(void* p, Azul::HeapNormal* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);

void* operator new[](size_t inSize, Azul::HeapNormal* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);
void operator delete[](void* p, Azul::HeapNormal* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);

// For Fixed Heap
void* operator new(size_t inSize, Azul::HeapFixed* const pHeap, const char* inName, size_t lineNum);
void operator delete(void* p, Azul::HeapFixed* const pHeap, const char* inName, size_t lineNum);

// -----------------------------------------------------------------
// Your new system needs to coexist with existing new/delete()
//    Simple new/delete is used in the unit test fixture
//    Welcome to memory system development
// -----------------------------------------------------------------
void *operator new( size_t inSize );
void operator delete( void *p );

void *operator new[](size_t inSize);
void operator delete[](void *p);

#endif

// --- End of File ---
