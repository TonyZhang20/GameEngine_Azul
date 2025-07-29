//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef DLINK_H
#define DLINK_H

#include "Manager_DLLInterface.h"

namespace Azul
{
	class DLink
	{
	public:

		MANAGER_LIBRARY_API DLink();
		MANAGER_LIBRARY_API DLink(const DLink &) = default;
		MANAGER_LIBRARY_API DLink &operator = (const DLink &) = default;
		MANAGER_LIBRARY_API virtual ~DLink() = default;

		MANAGER_LIBRARY_API virtual void Wash() = 0;

		// Becomes optional with a virtual with default implementation
		MANAGER_LIBRARY_API virtual char *GetName();

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pTargetNode) = 0;

		MANAGER_LIBRARY_API void Clear();
		MANAGER_LIBRARY_API virtual void Dump();
		MANAGER_LIBRARY_API virtual unsigned long GetHashCode() { return 0;  }

		// Data: -----------------------------
		DLink *pNext;
		DLink *pPrev;

	};
}

#endif

// --- End of File ---
