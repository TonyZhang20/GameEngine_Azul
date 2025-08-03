//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "DLink.h"

namespace Azul
{
	DLink::DLink()
	{
		this->Clear();
	}

	void DLink::Clear()
	{
		this->pNext = nullptr;
		this->pPrev = nullptr;
	}

	char *DLink::GetName()
	{
		return nullptr;
	}

	void DLink::Dump()
	{
		if (this->pPrev == nullptr)
		{
			Trace::out("      prev: null\n");
		}
		else
		{
			DLink *pTmp = this->pPrev;
			Trace::out("      prev: %s (%p)\n", pTmp->GetName(), pTmp);
		}

		Trace::out("      curr: %s (%p)\n", this->GetName(), this);

		if (this->pNext == nullptr)
		{
			Trace::out("      next: null\n");
		}
		else
		{
			DLink *pTmp = this->pNext;
			Trace::out("      next: %s (%p)\n", pTmp->GetName(), pTmp);
		}
	}
}

// --- End of File ---
