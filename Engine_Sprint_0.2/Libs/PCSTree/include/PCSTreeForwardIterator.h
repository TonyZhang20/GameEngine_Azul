//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef PCSTREE_FORWARD_ITERATOR_H
#define PCSTREE_FORWARD_ITERATOR_H

#include "PCSTreeIterator.h"
#include "PCSTree_DLLInterface.h"

namespace Azul
{
	class PCSTreeForwardIterator : public PCSTreeIterator
	{
	public:
		PCSTREE_LIBRARY_API PCSTreeForwardIterator(PCSNode *rootNode);

		PCSTREE_LIBRARY_API PCSTreeForwardIterator() = delete;
		PCSTREE_LIBRARY_API PCSTreeForwardIterator(const PCSTreeForwardIterator &);
		PCSTREE_LIBRARY_API PCSTreeForwardIterator &operator = (const PCSTreeForwardIterator &) = delete;
		PCSTREE_LIBRARY_API virtual ~PCSTreeForwardIterator() = default;

		PCSTREE_LIBRARY_API virtual PCSNode *First() override;
		PCSTREE_LIBRARY_API virtual PCSNode *Next() override;
		PCSTREE_LIBRARY_API virtual bool IsDone() override;
		PCSTREE_LIBRARY_API virtual PCSNode *Current() override;

	private:
		// YOU can store what you want... here
		// these were my variables...
		PCSNode *root;
		PCSNode *current;
	};

}

#endif

// ---  End of File ---------------