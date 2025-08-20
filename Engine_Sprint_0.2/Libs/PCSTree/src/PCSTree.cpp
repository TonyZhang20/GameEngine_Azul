//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTree.h"
#include "PCSNode.h"
#include "PCSTreeForwardIterator.h"

namespace Azul
{

	// constructor
	PCSTree::PCSTree()
	{
		this->pRoot = nullptr;
	};

	// destructor
	PCSTree::~PCSTree()
	{

	};

	// get Root
	PCSNode * PCSTree::GetRoot() const
	{
		return this->pRoot;
	}

	// insert
	void PCSTree::Insert(PCSNode * const pInNode, PCSNode * const pParent)
	{
		assert(pInNode);
		assert(pRoot == nullptr || pParent != nullptr);

		PCSNode* pChild = nullptr;

		if (this->pRoot == nullptr && pParent == nullptr)
		{
			this->pRoot = pInNode;
		}
		else if(pParent)
		{
			pChild = pParent->GetChild();

			pParent->SetChild(pInNode);
			pInNode->SetParent(pParent);
		}

		if (pChild)
		{
			//sibling
			pInNode->SetNextSibling(pChild);
			pChild->SetPrevSibling(pInNode);
		
			if (pParent != this->pRoot)
			{
				//dlink
				pInNode->SetReverseAuto(pParent);
			}
			else
			{
				pInNode->SetReverse(pChild->GetReverse());
			}

			pInNode->SetForwardAuto(pChild);
			pParent->SetForwardAuto(pInNode);
		}
		else if(pParent)
		{
			pParent->SetForwardAuto(pInNode);
		}


		this->mInfo.currNumNodes++;
		DataCheck(pInNode);

		PCSNode* forward = pInNode->GetForward();
		PCSNode* firstNode = pRoot;

		if (forward == nullptr)
		{
			firstNode->SetReverse(pInNode);
		}
	}

	PCSTREE_LIBRARY_API void PCSTree::InsertByOrder(PCSNode* const pInNode, PCSNode* const pPrevSib)
	{
		assert(pInNode);
		assert(pRoot);

		PCSNode* pParent = GetRoot();

		if (pPrevSib == nullptr)
		{
			this->Insert(pInNode, pParent);
			return;
		}

		pParent = pPrevSib->GetParent();

		//pParent->SetChild(pInNode);
		pInNode->SetParent(pParent);

		PCSNode* next = pPrevSib->GetNextSibling();

		if (next)
		{
			pInNode->SetNextSibling(next);
			next->SetPrevSibling(pInNode);
		
			pInNode->SetForwardAuto(next);
			next->SetReverseAuto(pInNode);
		}


		pPrevSib->SetNextSibling(pInNode);
		pInNode->SetPrevSibling(pPrevSib);


		//manage itr forward & Revserse
		pInNode->SetReverseAuto(pPrevSib);
		pPrevSib->SetForwardAuto(pInNode);

		this->mInfo.currNumNodes++;
		DataCheck(pInNode);

		if (this->pRoot->GetReverse() == pPrevSib)
		{
			this->pRoot->SetReverse(pInNode);
		}
	}

	// Remove
	void PCSTree::Remove(PCSNode * const pInNode)
	{
		bool result = VerfiyNode(pInNode);

		if (!result) return; //Invaild Remove
		
		PCSNode* parent = pInNode->GetParent();

		int reduceCount = pInNode->FindAllChilren() + 1;

		ClearNodeData(pInNode);

		if (pRoot)
		{
			this->mInfo.currNumLevels = pRoot->GetMaxChildLevel();
		}
		else
		{
			this->mInfo.currNumLevels = 0;
		}
		
		this->mInfo.currNumNodes -= reduceCount;

		CheckMax();
	}

	void PCSTree::ClearNodeData(PCSNode* pInNode)
	{
		PCSNode* parent = pInNode->GetParent();

		PCSNode* nextSib = pInNode->GetNextSibling();
		PCSNode* prevSib = pInNode->GetPrevSibling();

		if (prevSib)
			prevSib->SetNextSibling(nextSib);

		if (nextSib)
			nextSib->SetPrevSibling(prevSib);

		if (parent && parent->GetChild() == pInNode)
		{
			parent->SetChild(nextSib);
		}

		PCSNode* current = pInNode->GetChild();

		if (current)
		{
			current->SetParent(nullptr);
		}

		while (current)
		{
			if (current->GetChild())
			{
				current = current->GetChild();
			}
			else if (current->GetNextSibling())
			{
				current = current->GetNextSibling();
			}
			else
			{
				PCSNode* washNode = current;

				if (washNode->GetPrevSibling())
				{
				
					current = washNode->GetPrevSibling();
					current->SetNextSibling(nullptr);
				
				}
				else if (washNode->GetParent())
				{

					current = washNode->GetParent();
					current->SetChild(nullptr);
				}
				else //Cant find anything, I am the last node
				{
					ClearDLink(washNode);
					washNode->Wash();
					break;
				}

				ClearDLink(washNode);
				washNode->Wash();
			}
		}

		ClearDLink(pInNode);
		pInNode->Wash();

		if (pInNode == pRoot)
			pRoot = nullptr;
	}

	// get tree info
	void PCSTree::GetInfo(Info & info)
	{
		info = this->mInfo;
	}

	void PCSTree::Print() const
	{
		Trace::out2("\n\n----------------Print Tree---------------\n");
		PCSTreeForwardIterator itr(this->pRoot);
		for (itr.First(); !itr.IsDone(); itr.Next())
		{
			itr.Current()->PrintNode();
		}
		Trace::out2("----------------End Print---------------\n");
	}

	void PCSTree::CheckMax()
	{
		if (this->mInfo.currNumNodes > this->mInfo.maxNumNodes)
		{
			this->mInfo.maxNumNodes = this->mInfo.currNumNodes;
		}

		if (this->mInfo.currNumLevels > this->mInfo.maxNumLevels)
		{
			this->mInfo.maxNumLevels = this->mInfo.currNumLevels;
		}
	}

	void PCSTree::DataCheck(PCSNode* const pInNode)
	{
		int count = 0;

		PCSNode* tmpNode = pInNode;

		while (tmpNode)
		{
			count++;
			tmpNode = tmpNode->GetParent();
		}

		if (count > mInfo.currNumLevels)
		{
			mInfo.currNumLevels = count;
		}

		CheckMax();
	}

	bool PCSTree::VerfiyNode(PCSNode* pInNode) const
	{
		assert(pInNode);

		PCSNode* pCurrent = pInNode;

		while (true)
		{
			if (pCurrent->GetParent() != nullptr) //UP first
			{
				pCurrent = pCurrent->GetParent();
			}
			else
			{
				break;
			}
		}


		if (pRoot == pCurrent)
			return true;


		return false;
	}

	void PCSTree::LateNodeCheck(PCSNode* pInNode)
	{
		PCSNode* firstNode = pRoot;

		//ReverseNode Lock
		if (pInNode->GetForward() == nullptr)
		{
			if (firstNode->GetReverse() == pInNode)
			{
				firstNode->SetReverse(pInNode->GetReverse());
			}
		}
	}

	void PCSTree::ClearDLink(PCSNode* pInNode)
	{
		LateNodeCheck(pInNode);

		PCSNode* forward = pInNode->GetForward();
		PCSNode* reverse = pInNode->GetReverse();

		if (forward == pInNode)
		{
			return;
		}

		if (reverse)
		{
			reverse->SetForward(forward);
		}

		if (forward)
		{
			forward->SetReverse(reverse);
		}

	}

	PCSTREE_LIBRARY_API void PCSTree::SetRoot(PCSNode* pRoot)
	{
		this->pRoot = pRoot;
	}

	PCSNode* PCSTree::RemoveFromLast()
	{
		PCSNode* node = nullptr;

		if (pRoot)
		{
			if (pRoot->GetReverse())
			{
				node = pRoot->GetReverse();
			}
			else
			{
				node = pRoot;
			}

			this->Remove(node);
		}

		return node;
	}
}

// ---  End of File ---
