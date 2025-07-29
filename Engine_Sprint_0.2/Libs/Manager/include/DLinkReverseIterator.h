//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef	DLINK_REVERSE_ITERATOR_H
#define	DLINK_REVERSE_ITERATOR_H

#include "Manager_DLLInterface.h"
#include "Iterator.h"

namespace Azul
{
	class DLinkReverseIterator : public Iterator
	{
	public:
		MANAGER_LIBRARY_API DLinkReverseIterator();

		MANAGER_LIBRARY_API virtual DLink* Next() override;
		MANAGER_LIBRARY_API virtual bool IsDone() override;
		MANAGER_LIBRARY_API virtual DLink* First() override;
		MANAGER_LIBRARY_API virtual DLink* Curr() override;

		MANAGER_LIBRARY_API void Reset(DLink* pEnd);

	private:
		void privClear();

	public:
		DLink* pEnd;
		DLink* pCurr;
		bool bDone;
	};
}

#endif

// --- End of File ---

