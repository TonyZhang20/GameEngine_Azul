//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef PCSTREE_REVERSE_ITERATOR_H
#define PCSTREE_REVERSE_ITERATOR_H

#include "PCSTreeIterator.h"
#include "PCSTree_DLLInterface.h"

namespace Azul
{
	class PCSTreeReverseIterator : public PCSTreeIterator
	{
	public:

		PCSTREE_LIBRARY_API PCSTreeReverseIterator(PCSNode *rootNode);

		PCSTREE_LIBRARY_API PCSTreeReverseIterator() = delete;
		PCSTREE_LIBRARY_API PCSTreeReverseIterator(const PCSTreeReverseIterator &);
		PCSTREE_LIBRARY_API PCSTreeReverseIterator &operator = (const PCSTreeReverseIterator &) = delete;
		PCSTREE_LIBRARY_API virtual ~PCSTreeReverseIterator() = default;

		PCSTREE_LIBRARY_API virtual PCSNode *First() override;
		PCSTREE_LIBRARY_API virtual PCSNode *Next() override;
		PCSTREE_LIBRARY_API virtual bool IsDone() override;
		PCSTREE_LIBRARY_API virtual PCSNode *Current() override;

	private:
		// YOU can store what you want... here
		// these were my variables...
		PCSNode *root;
		PCSNode *current;
		bool visited = false;
	};

}

#endif

// ---  End of File ---------------