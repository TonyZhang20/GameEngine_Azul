//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef PCSNODE_H
#define PCSNODE_H

#include "PCSTree_DLLInterface.h"

namespace Azul
{
	class PCSNode
	{
	public:
		// Constants for the library
		static const unsigned int NAME_SIZE = 32;
		static const unsigned int MAJOR_VERSION = 6;
		static const unsigned int MINOR_VERSION = 0;

	public:
		// Enums
		enum class Code : uint32_t
		{
			SUCCESS,
			FAIL_NULL_PTR,
			FAIL_NAME_MISMATCH,
			FAIL_RETURN_NOT_INITIALIZED
		};


	public:
		// constructor
		PCSTREE_LIBRARY_API PCSNode();

		// copy constructor
		PCSTREE_LIBRARY_API PCSNode(const PCSNode &in);

		// Specialize Constructor
		PCSTREE_LIBRARY_API PCSNode(PCSNode *const pInParent,
			PCSNode *const pInChild,
			PCSNode *const pInNextSibling,
			PCSNode *const pInPrevSibling,
			const char *const pInName);

		PCSTREE_LIBRARY_API PCSNode(const char *const pInName);

		// destructor
		PCSTREE_LIBRARY_API virtual ~PCSNode() = default;

		// assignment operator
		PCSTREE_LIBRARY_API PCSNode &operator = (const PCSNode &in);

		// accessors
		PCSTREE_LIBRARY_API void SetParent(PCSNode *const pIn);
		PCSTREE_LIBRARY_API void SetChild(PCSNode *const pIn);
		PCSTREE_LIBRARY_API void SetNextSibling(PCSNode *const pIn);
		PCSTREE_LIBRARY_API void SetPrevSibling(PCSNode *const pIn);

		PCSTREE_LIBRARY_API void SetForwardAuto(PCSNode* const pIn);
		PCSTREE_LIBRARY_API void SetReverseAuto(PCSNode* const pIn);

		PCSTREE_LIBRARY_API PCSNode *GetParent(void) const;
		PCSTREE_LIBRARY_API PCSNode *GetChild(void) const;
		PCSTREE_LIBRARY_API PCSNode *GetNextSibling(void) const;
		PCSTREE_LIBRARY_API PCSNode *GetPrevSibling(void) const;
		PCSTREE_LIBRARY_API PCSNode *GetForward(void) const;
		PCSTREE_LIBRARY_API PCSNode *GetReverse(void) const;

		// name
		PCSTREE_LIBRARY_API Code SetName(const char *const pInName);
		PCSTREE_LIBRARY_API Code GetName(char *const pOutBuffer, unsigned int sizeOutBuffer) const;
		PCSTREE_LIBRARY_API Code CampareName(const char* pInName);
		// print
		PCSTREE_LIBRARY_API void PrintNode() const;
		PCSTREE_LIBRARY_API void PrintChildren() const;
		PCSTREE_LIBRARY_API void PrintSiblings() const;

		// get number of children/siblings
		PCSTREE_LIBRARY_API int GetNumSiblings() const;
		PCSTREE_LIBRARY_API int FindAllChilren() const;
		PCSTREE_LIBRARY_API int GetNumChildren() const;
		PCSTREE_LIBRARY_API int GetMaxChildLevel() const;

		PCSTREE_LIBRARY_API void Wash();

		PCSTREE_LIBRARY_API virtual bool Compare(PCSNode* pTargetNode) = 0;
		PCSTREE_LIBRARY_API virtual unsigned long GetHashCode();
		PCSTREE_LIBRARY_API virtual int GetOrder() { return 0; }

	protected:
		PCSTREE_LIBRARY_API void SetForward(PCSNode* const pIn);
		PCSTREE_LIBRARY_API void SetReverse(PCSNode* const pIn);

	protected:
		char    pName[PCSNode::NAME_SIZE];

	private:
		PCSNode *pParent;
		PCSNode *pChild;
		PCSNode *pNextSibling;
		PCSNode *pPrevSibling;
		PCSNode *pForward;
		PCSNode *pReverse;

		friend class PCSTree;
	};

}

#endif

// ---  End of File ---
