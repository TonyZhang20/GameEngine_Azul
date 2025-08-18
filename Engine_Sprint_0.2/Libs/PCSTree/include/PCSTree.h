//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef PCSTREE_H
#define PCSTREE_H
#include "PCSTree_DLLInterface.h"
namespace Azul
{

	// forward declare
	class PCSNode;

	// PCSTree class 
	class PCSTree
	{
	public:
		// Simple data structure
		struct Info
		{
			int currNumNodes;
			int maxNumNodes;
			int currNumLevels;
			int maxNumLevels;
		};

	public:
		// constructor
		PCSTREE_LIBRARY_API PCSTree();

		// copy constructor 
		PCSTREE_LIBRARY_API PCSTree(const PCSTree &in) = delete;

		// assignment operator
		PCSTREE_LIBRARY_API PCSTree &operator = (const PCSTree &in) = delete;

		// destructor
		PCSTREE_LIBRARY_API virtual ~PCSTree();

		// get Root
		PCSTREE_LIBRARY_API PCSNode *GetRoot() const;

		// insert
		PCSTREE_LIBRARY_API void Insert(PCSNode *const pInNode, PCSNode *const pParent);
		/// <summary>
		/// Only will set the parent as root
		/// </summary>
		/// <returns></returns>
		PCSTREE_LIBRARY_API void InsertByOrder(PCSNode* const pInNode, PCSNode *const pPrevSib);

		// remove
		PCSTREE_LIBRARY_API void Remove(PCSNode *const pInNode);
		PCSTREE_LIBRARY_API virtual void ClearNodeData(PCSNode* pInNode);

		// get info
		PCSTREE_LIBRARY_API void GetInfo(Info &info);
		PCSTREE_LIBRARY_API void Print() const;
		PCSTREE_LIBRARY_API void CheckMax();
		PCSTREE_LIBRARY_API void DataCheck(PCSNode* const pInNode);

		PCSTREE_LIBRARY_API bool VerfiyNode(PCSNode* pInNode) const;

		PCSTREE_LIBRARY_API void LateNodeCheck(PCSNode* pInNode);
		PCSTREE_LIBRARY_API void ClearDLink(PCSNode* pInNode);
		PCSTREE_LIBRARY_API void SetRoot(PCSNode* pRoot);
		PCSTREE_LIBRARY_API PCSNode* RemoveFromLast();


	private:
                // feel free to add helper methods

	private:
		// Data
		Info	mInfo;
		PCSNode *pRoot;

	};

}

#endif

// ---  End of File ---
