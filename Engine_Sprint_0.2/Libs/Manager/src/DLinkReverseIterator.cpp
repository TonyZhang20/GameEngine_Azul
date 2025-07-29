//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "DLinkReverseIterator.h"

namespace Azul
{
	DLinkReverseIterator::DLinkReverseIterator()
		: Iterator()
	{
		this->privClear();
	}

	void DLinkReverseIterator::privClear()
	{
		this->pCurr = nullptr;
		this->pEnd = nullptr;
		this->bDone = true;
	}

	DLink* DLinkReverseIterator::Next()
	{
		DLink* pLink = this->pCurr;

		if (pLink != nullptr)
		{
			pLink = pLink->pPrev;
		}

		this->pCurr = pLink;

		if (pLink == nullptr)
		{
			bDone = true;
		}
		else
		{
			bDone = false;
		}

		return pLink;
	}

	bool DLinkReverseIterator::IsDone()
	{
		return bDone;
	}

	DLink* DLinkReverseIterator::First()
	{
		// this should give the first node
		// essentially reset the list
		if (this->pEnd != nullptr)
		{
			this->bDone = false;
			this->pCurr = this->pEnd;
		}
		else
		{
			this->privClear();
		}

		return pCurr;
	}

	DLink* DLinkReverseIterator::Curr()
	{
		DLink* pNode = this->pCurr;
		return pNode;
	}

	void DLinkReverseIterator::Reset(DLink* _pEnd)
	{
		if (_pEnd != nullptr)
		{
			this->bDone = false;
			this->pCurr = _pEnd;
			this->pEnd = _pEnd;
		}
		else
		{
			this->privClear();
		}
	}
}

// --- End of File ---
