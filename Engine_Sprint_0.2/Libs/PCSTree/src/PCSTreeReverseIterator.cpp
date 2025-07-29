//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTreeReverseIterator.h"

namespace Azul
{
	PCSTreeReverseIterator::PCSTreeReverseIterator(PCSNode *rootNode)
	{
		this->root = rootNode;
	}

	PCSNode *PCSTreeReverseIterator::First()
	{
		visited = false;

		if (!root)
		{
			visited = true;
			current = nullptr;

			return nullptr;
		}

		current = root->GetReverse();

		return current;
	}

	PCSNode *PCSTreeReverseIterator::Next()
	{
		current = current->GetReverse();

		if (current == root->GetReverse())
		{
			visited = true;
		}

		return current;
	}

	bool PCSTreeReverseIterator::IsDone()
	{
		return !current || visited;
	}

	PCSNode *PCSTreeReverseIterator::Current()
	{
		return current;
	}

}

// ---  End of File ---
