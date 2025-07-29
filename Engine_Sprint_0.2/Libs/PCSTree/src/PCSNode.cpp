//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "PCSTree.h"
#include "PCSNode.h"

namespace Azul
{
	// constructor
	PCSNode::PCSNode():
		pParent(nullptr), pChild(nullptr), pNextSibling(nullptr), pPrevSibling(nullptr),
		pForward(nullptr), pReverse(nullptr)
	{

	}

	// copy constructor
	PCSNode::PCSNode(const PCSNode & in):
		pParent(in.pParent), pChild(in.pChild), pNextSibling(in.pNextSibling), pPrevSibling(in.pPrevSibling),
		pForward(in.pForward), pReverse(in.pPrevSibling)
	{
		this->SetName(in.pName);
	}

	// Specialize Constructor
	PCSNode::PCSNode(PCSNode * const pInParent, PCSNode * const pInChild, PCSNode * const pInNextSibling, PCSNode * const pInPrevSibling, const char * const pInName)
	{
		this->pChild = pInChild;
		this->pParent = pInParent;

		this->pNextSibling = pInNextSibling;
		this->pPrevSibling = pInPrevSibling;

		this->SetName(pInName);
	}

	PCSNode::PCSNode(const char * const pInName)
		: pParent(nullptr), pChild(nullptr), pNextSibling(nullptr), pPrevSibling(nullptr),
		pForward(nullptr), pReverse(nullptr)
	{
		this->SetName(pInName);
	}

	// assignment operator
	PCSNode & PCSNode::operator = (const PCSNode & in)
	{
		this->pChild = in.pChild;
		this->pParent = in.pParent;

		this->pNextSibling = in.pNextSibling;
		this->pPrevSibling = in.pPrevSibling;

		this->pForward = in.pForward;
		this->pReverse = in.pReverse;

		this->SetName(in.pName);
		return *this;
	}

	void PCSNode::SetParent(PCSNode * const pIn)
	{
		this->pParent = pIn;
	}

	void PCSNode::SetChild(PCSNode * const pIn)
	{
		pChild = pIn;
	}

	void PCSNode::SetNextSibling(PCSNode * const pIn)
	{
		this->pNextSibling = pIn;
	}

	void PCSNode::SetPrevSibling(PCSNode * const pIn)
	{
		this->pPrevSibling = pIn;
	}

	void PCSNode::SetForwardAuto(PCSNode* const pIn)
	{
		if (this->pForward && this->pForward != pIn)
		{
			this->pForward->pReverse = pIn;
			if (pIn)	pIn->pForward = this->pForward;
		}

		if (pIn) pIn->pReverse = this;
		this->pForward = pIn;
	}

	void PCSNode::SetReverseAuto(PCSNode* const pIn)
	{
		if (this->pReverse && this->pReverse != pIn)
		{
			this->pReverse->pForward = pIn;
			if (pIn)	pIn->pReverse = this->pReverse;
		}

		if (pIn) pIn->pForward = this;
		this->pReverse = pIn;
	}

	void PCSNode::SetForward(PCSNode* const pIn)
	{
		this->pForward = pIn;
	}

	void PCSNode::SetReverse(PCSNode* const pIn)
	{
		this->pReverse = pIn;
	}

	PCSNode * PCSNode::GetParent(void) const
	{
		return this->pParent;
	}

	PCSNode * PCSNode::GetChild(void) const
	{
		return this->pChild;
	}

	PCSNode * PCSNode::GetNextSibling(void) const
	{
		return this->pNextSibling;
	}

	PCSNode * PCSNode::GetPrevSibling(void) const
	{
		return this->pPrevSibling;
	}

	PCSNode* PCSNode::GetForward(void) const
	{
		return this->pForward;
	}

	PCSNode* PCSNode::GetReverse(void) const
	{
		return this->pReverse;
	}

	PCSNode::Code PCSNode::SetName(const char * const pInName)
	{
		if (pInName == nullptr) return Code::FAIL_NULL_PTR;

		strncpy_s(this->pName, pInName, NAME_SIZE - 1);
		this->pName[NAME_SIZE - 1] = '\0';

		return Code::SUCCESS;
	}

	PCSNode::Code PCSNode::GetName(char * const pOutBuffer, unsigned int sizeOutBuffer) const
	{
		if (pOutBuffer == nullptr)
		{
			return Code::FAIL_NULL_PTR;
		}

		memcpy(pOutBuffer, this->pName, sizeOutBuffer);
		pOutBuffer[sizeOutBuffer - 1] = '\0';

		return Code::SUCCESS;
	}

	PCSNode::Code PCSNode::CampareName(const char* pInName)
	{
		if (pInName == nullptr)
		{
			return Code::FAIL_NULL_PTR;
		}

		if (this->pName == nullptr)
		{
			return Code::FAIL_NULL_PTR;
		}

		if (strcmp(this->pName, pInName) == 0)
		{
			return Code::SUCCESS;
		}
		else
		{
			return Code::FAIL_NAME_MISMATCH;
		}
	}

	void PCSNode::PrintNode() const
	{
		Trace::out2("Node is %s \n", this->pName);
	}

	void PCSNode::PrintChildren() const
	{
		if(pChild)
			this->pChild->PrintNode();
	}

	void PCSNode::PrintSiblings() const
	{
		if(pPrevSibling)
			this->pPrevSibling->PrintNode();
	}

	int PCSNode::GetNumSiblings() const
	{
		int count = 1;
		//Prev
		PCSNode* tmp = pPrevSibling;
		while (tmp != nullptr)
		{
			count++;
			tmp = tmp->pPrevSibling;
		}

		tmp = pNextSibling;

		while (tmp != nullptr)
		{
			count++;
			tmp = tmp->pNextSibling;
		}

		return count;
	}

	int PCSNode::FindAllChilren() const
	{
		int count = 0;

		const PCSNode* current = this->pChild;

		while (current)
		{
			count++;

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
				
				while (current && !current->GetNextSibling())
				{
					current = current->GetParent();

					if (current == this) 
						return count;
				}

				if (current)
					current = current->GetNextSibling();
			}
		}

		return count;
	}

	int PCSNode::GetNumChildren() const
	{
		int count = 0;
		PCSNode* tmpNode = this->pChild;

		while (tmpNode != nullptr)
		{
			count++;
			tmpNode = tmpNode->GetNextSibling();
		}

		return count;
	}

	int PCSNode::GetMaxChildLevel() const
	{
		const PCSNode* current = this;

		int currentDepth = 1;
		int maxDepth = 1;

		while (current)
		{
			//child first
			if (current->GetChild())
			{
				current = current->GetChild();
				currentDepth++;
			}
			//sibling second
			else if (current->GetNextSibling())
			{
				current = current->GetNextSibling();
			}
			// going back
			else
			{
				//back to top who has sibling
				while (current && !current->GetNextSibling())
				{
					current = current->GetParent();
					currentDepth--;
				}

				if (current)
				{
					current = current->GetNextSibling();
				}
			}

			if (currentDepth >= maxDepth)
					maxDepth = currentDepth;
		}

		return maxDepth;
	}

	void PCSNode::Wash()
	{
		this->pParent = nullptr;
		this->pChild = nullptr;

		this->pPrevSibling = nullptr;
		this->pNextSibling = nullptr;

		this->pForward = nullptr;
		this->pReverse = nullptr;

		this->SetName("");
	}

	unsigned long PCSNode::GetHashCode()
	{
		unsigned long hash = 5381;
		size_t i = 0;

		while (i < NAME_SIZE && pName[i] != '\0')
		{
			hash = ((hash << 5) + hash) + pName[i]; // hash * 33 + str[i]
			++i;
		}

		return hash;
	}
}

// ---  End of File ---
