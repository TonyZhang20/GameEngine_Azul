#include <cstring>
#include "Layer.h"

namespace Azul
{
	Layer::Layer(const char* name) : PCSNode(name)
	{
	}

	Layer::~Layer()
	{
	}

	bool Layer::Compare(PCSNode* pTargetNode)
	{
		return this->GetHashCode() == pTargetNode->GetHashCode();
	}

	void Layer::SetOrder(int _order)
	{
		this->order = _order;
	}

	void Layer::Set(const char* const name, const int order)
	{
		this->SetName(name);
		this->SetOrder(order);
	}

}