//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef DLINK_MAN_H
#define DLINK_MAN_H

#include "Manager_DLLInterface.h"
#include "ListBase.h"
#include "DLinkIterator.h"
#include "DLinkReverseIterator.h"
namespace Azul
{
	class DLinkMan : public ListBase
	{
	public:
		MANAGER_LIBRARY_API DLinkMan();
		MANAGER_LIBRARY_API DLinkMan(const DLinkMan &) = default;
		MANAGER_LIBRARY_API DLinkMan &operator = (const DLinkMan &) = default;
		MANAGER_LIBRARY_API virtual ~DLinkMan();

		MANAGER_LIBRARY_API virtual void AddToFront(DLink *pNode) override;

		MANAGER_LIBRARY_API virtual void AddToEnd(DLink *pNode) override;

		MANAGER_LIBRARY_API virtual void Remove(DLink *pNode) override;

		MANAGER_LIBRARY_API virtual DLink *RemoveFromFront() override;

		MANAGER_LIBRARY_API virtual Iterator *GetIterator() override;
		MANAGER_LIBRARY_API virtual Iterator *GetReverseIterator() override;

		DLinkIterator *poIterator;
		DLinkReverseIterator* poReverseIterator;

		DLink *poHead;
		DLink* poEnd;

	};
}

#endif

// --- End of File ---
