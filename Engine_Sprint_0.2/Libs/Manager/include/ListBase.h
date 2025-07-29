//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef LIST_BASE_H
#define LIST_BASE_H

#include "Manager_DLLInterface.h"
#include "DLink.h"
#include "Iterator.h"

namespace Azul
{
	class ListBase
	{
	public:
		MANAGER_LIBRARY_API ListBase() = default;
		MANAGER_LIBRARY_API ListBase(const ListBase &) = default;
		MANAGER_LIBRARY_API ListBase &operator = (const ListBase &) = default;
		MANAGER_LIBRARY_API virtual ~ListBase() = default;

		MANAGER_LIBRARY_API virtual void AddToFront(DLink *pNode) = 0;
		MANAGER_LIBRARY_API virtual void AddToEnd(DLink *pNode) = 0;
		MANAGER_LIBRARY_API virtual void Remove(DLink *pNode) = 0;
		MANAGER_LIBRARY_API virtual DLink *RemoveFromFront() = 0;

		MANAGER_LIBRARY_API virtual Iterator *GetIterator() = 0;
		MANAGER_LIBRARY_API virtual Iterator* GetReverseIterator() { assert(false); return nullptr; };
	};
}

#endif

// --- End of File ---