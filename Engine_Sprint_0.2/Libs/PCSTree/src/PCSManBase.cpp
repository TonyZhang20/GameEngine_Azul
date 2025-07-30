#include "PCSManBase.h"
#include "PCSNode.h"
#include "PCSTree.h"

namespace Azul
{
	PCSManBase::PCSManBase(PCSTree* _poActive, PCSTree* _poReserve, int InitialNumReserved, int DeltaGrow)
	{
		// Check now or pay later
		assert(_poActive != nullptr);
		assert(_poReserve != nullptr);
		AZUL_UNUSED_VAR(InitialNumReserved);
		assert(InitialNumReserved >= 0);
		assert(DeltaGrow > 0);

		// Initialize all variables
		this->mDeltaGrow = DeltaGrow;
		this->mNumReserved = 0;
		this->mNumActive = 0;
		this->mTotalNumNodes = 0;
		this->poActive = _poActive;
		this->poReserve = _poReserve;

	}

	PCSManBase::~PCSManBase()
	{

		baseDestroy();

		PCSNode* ActiveRoot = poActive->GetRoot();
		PCSNode* ReverseRoot = poReserve->GetRoot();

		if(this->poActive) delete this->poActive;
		if(this->poReserve) delete this->poReserve;

		if (ActiveRoot)
		{
			//ActiveRoot->PrintNode();
			delete ActiveRoot;
		}

		if (ReverseRoot) delete ReverseRoot;

		this->poActive = nullptr;
		this->poReserve = nullptr;
	}

	void PCSManBase::baseSetReserve(int reserveNum, int reserveGrow)
	{
		this->mDeltaGrow = reserveGrow;

		if (reserveNum > this->mNumReserved)
		{
			// Preload the reserve
			this->proFillReservedPool(reserveNum - this->mNumReserved);
		}
	}

	PCSNode* PCSManBase::baseAdd(PCSNode* parent)
	{
		PCSTreeForwardIterator pForIter(poReserve->GetRoot());

		if (pForIter.First() == nullptr)
		{
			// refill the reserve list by the DeltaGrow
			this->proFillReservedPool(this->mDeltaGrow);
		}

		if (parent == nullptr)
		{
			parent = poActive->GetRoot();
		}

		// Always take from the reserve list
		PCSNode* pNodeBase = poReserve->RemoveFromLast();
		assert(pNodeBase != nullptr);

		// Wash it
		pNodeBase->Wash();

		this->mNumActive++;
		this->mNumReserved--;

		poActive->Insert(pNodeBase, parent);

		return pNodeBase;
	}

	PCSNode* PCSManBase::GetEmptyNodeFromPool()
	{
		PCSTreeForwardIterator pForIter(poReserve->GetRoot());

		if (pForIter.First() == nullptr)
		{
			// refill the reserve list by the DeltaGrow
			this->proFillReservedPool(this->mDeltaGrow);
		}

		// Always take from the reserve list
		PCSNode* pNodeBase = poReserve->RemoveFromLast();
		assert(pNodeBase != nullptr);

		pNodeBase->Wash();

		this->mNumActive++;
		this->mNumReserved--;

		return pNodeBase;
	}

	PCSNode* PCSManBase::baseFind(PCSNode* pNodeTarget)
	{
		PCSNode* pNode = nullptr;

		PCSTreeForwardIterator pIt(poActive->GetRoot());

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pNode = pIt.Current();
			if (pNode->Compare(pNodeTarget))
			{
				// found it
				break;
			}
		}

		return pNode;
	}

	void PCSManBase::baseRemove(PCSNode* pNodeBase)
	{
		this->poActive->Remove(pNodeBase);

		pNodeBase->Wash();

		// add it to the return list
		poReserve->Insert(pNodeBase, poReserve->GetRoot());

		// stats update
		this->mNumActive--;
		this->mNumReserved++;

	}

	void PCSManBase::baseDump()
	{
		Trace::out("\n");
		Trace::out("   --- %s: Begin --- \n", STRING_ME(ManBase));
		Trace::out("\n");

		Trace::out("         mDeltaGrow: %d \n", mDeltaGrow);
		Trace::out("     mTotalNumNodes: %d \n", mTotalNumNodes);
		Trace::out("       mNumReserved: %d \n", mNumReserved);
		Trace::out("         mNumActive: %d \n", mNumActive);
		Trace::out("\n");

		Trace::out("    Active : \n");
		poActive->Print();

		Trace::out("    Reserve : \n");
		poReserve->Print();
	}

	void PCSManBase::baseDestroy()
	{
		PCSTreeReverseIterator pActiveForItr(poActive->GetRoot());

		PCSNode* pTmp = nullptr;
		PCSNode* pNode = nullptr;

		pNode = pActiveForItr.First();
		
		while (!pActiveForItr.IsDone())
		{
			pTmp = pActiveForItr.Current();

			pNode = pActiveForItr.Next();

			//pTmp->PrintNode();

			delete pTmp;
			
			if (pNode == poActive->GetRoot()) break;
		}

		PCSTreeReverseIterator pReserveForItr(poReserve->GetRoot());

		pNode = pReserveForItr.First();

		while (!pReserveForItr.IsDone())
		{
			pTmp = pReserveForItr.Current();

			pNode = pReserveForItr.Next();

			//pTmp->PrintNode();
			delete pTmp;

			if (pNode == poReserve->GetRoot()) break;
		}

	}

	void PCSManBase::proFillReservedPool(int count)
	{
		assert(count >= 0);

		this->mTotalNumNodes += count;
		this->mNumReserved += count;

		// Preload the reserve
		for (int i = 0; i < count; i++)
		{
			PCSNode* pNode = this->derivedCreateNode();
			assert(pNode != nullptr);

			poReserve->Insert(pNode, poReserve->GetRoot());
		}
	}

}


