//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MeshNodeManager.h"
#include "MeshNull.h"
#include "ManBase.h"
#include "DLinkMan.h"

namespace Azul
{
	MeshNodeManager* MeshNodeManager::posInstance = nullptr;

	//----------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------
	MeshNodeManager::MeshNodeManager(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		// Preload the reserve
		this->proFillReservedPool(reserveNum);

		// initialize derived data here
		Mesh* pMesh = new MeshNull();
		assert(pMesh);
		this->poNodeCompare = new MeshNode();
		assert(this->poNodeCompare);
		this->poNodeCompare->Set(Mesh::Name::NULL_MESH, pMesh);
	}

	MeshNodeManager::~MeshNodeManager()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		// iterate through the list and delete
		Iterator* pIt = this->baseGetActiveIterator();

		DLink* pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			DLink* pDeleteMe = pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			DLink* pDeleteMe = pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}
	}

	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
	void MeshNodeManager::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(posInstance == nullptr);

		// Do the initialization
		if (posInstance == nullptr)
		{
			posInstance = new MeshNodeManager(reserveNum, reserveGrow);
		}
	}

	void MeshNodeManager::Destroy()
	{
		MeshNodeManager* pMan = MeshNodeManager::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete MeshNodeManager::posInstance;
		MeshNodeManager::posInstance = nullptr;
	}

	MeshNode* MeshNodeManager::Add(Mesh::Name name, Mesh* pMesh)
	{
		MeshNodeManager* pMan = MeshNodeManager::privGetInstance();

		assert(pMesh);

		MeshNode* pNode = (MeshNode*)pMan->baseAddToFront();
		assert(pNode != nullptr);

		// Initialize the date
		pNode->Set(name, pMesh);

		return pNode;
	}

	MeshNode* MeshNodeManager::Find(Mesh::Name _name)
	{
		MeshNodeManager* pMan = MeshNodeManager::privGetInstance();
		assert(pMan != nullptr);

		// Compare functions only compares two Nodes

		// So:  Use the Compare Node - as a reference
		//      use in the Compare() function
		Mesh* pMesh = pMan->poNodeCompare->GetMesh();
		assert(pMesh);

		pMesh->name = _name;

		MeshNode* pData = (MeshNode*)pMan->baseFind(pMan->poNodeCompare);
		return pData;
	}

	void MeshNodeManager::Remove(MeshNode* pNode)
	{
		assert(pNode != nullptr);

		MeshNodeManager* pMan = MeshNodeManager::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseRemove(pNode);
	}

	void MeshNodeManager::Dump()
	{
		MeshNodeManager* pMan = MeshNodeManager::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
	MeshNodeManager* MeshNodeManager::privGetInstance()
	{
		// Safety - this forces users to call Create() first before using class
		assert(posInstance != nullptr);
		return posInstance;
	}

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
	DLink* MeshNodeManager::derivedCreateNode()
	{
		DLink* pNodeBase = new MeshNode();
		assert(pNodeBase != nullptr);

		return pNodeBase;
	}
}




// --- End of File ---




