#include "LayerManager.h"
#include <cstring> 

namespace Azul
{
	LayerManager* LayerManager::instance = nullptr;

	void LayerManager::Create(int _reserveNum, int _reserveGrow)
	{
		assert(_reserveNum >= 0);
		assert(_reserveGrow > 0);

		if (instance == nullptr)
		{
			LayerManager::instance = new LayerManager(_reserveNum, _reserveGrow);
		}
		else
		{
			assert(false);
		}


		LayerManager* pMan = LayerManager::instance;

		Layer* rootNode = (Layer*)pMan->GetEmptyNodeFromPool();

		rootNode->Set("Root", 0);

		pMan->GetActiveLayers()->Insert(rootNode, nullptr);

		assert(instance);
	}

	void LayerManager::Destroy()
	{
		if (LayerManager::instance != nullptr)
		{
			delete LayerManager::instance;
			LayerManager::instance = nullptr;
		}
	}

	LayerManager::LayerManager(int _reserveNum, int _reserveGrow)
		: PCSManBase(new PCSTree(), new PCSTree(), _reserveNum, _reserveGrow)
	{

	}

	LayerManager* LayerManager::privGetInstance()
	{
		return instance;
	}

	void LayerManager::Add(Layer* pInNode, Layer* parent, int order)
	{
		assert(pInNode);

		// Get singleton
		LayerManager* pGOM = LayerManager::privGetInstance();

		if (parent == nullptr)
		{
			parent = (Layer*)pGOM->poActive->GetRoot();

			pGOM->poActive->Insert(pInNode, parent);
		}
		else
		{
			assert(false);
		}
	}

	void LayerManager::AddByOrder(Layer* pInNode)
	{
		LayerManager* instance = LayerManager::privGetInstance();

		PCSTree* poTree = instance->poActive;
		PCSNode* tmpNode = poTree->GetRoot()->GetChild();

		Layer* prevNode = nullptr;
		Layer* layer = nullptr;

		int order = pInNode->GetOrder();

		bool find = false;

		//Try find same order
		while (tmpNode != nullptr)
		{
			layer = (Layer*) tmpNode;
			if (layer->GetOrder() == order)
			{
				find = true;
				break;
			}
			else if (layer->GetOrder() < order)
			{
				prevNode = layer;
			}
			else
			{
				break;
			}

			tmpNode = tmpNode->GetNextSibling();
		}


		if (find) //Find same order layer
		{
			//Layer is parent
			//Add as child
			instance->poActive->Insert(pInNode, layer);
			return;
		}
		else 
		{
			//Create Empty Node for CurrentLayer
			//Get EMpty Node
			//Layer* objNode = (Layer*)instance->GetEmptyNodeFromPool();

			//Add behind
			instance->poActive->InsertByOrder(pInNode, prevNode);
			return;
		}
	}

	PCSTreeForwardIterator LayerManager::GetForwardActiveIterator()
	{
		LayerManager* instance = LayerManager::privGetInstance();
		assert(instance);
		
		PCSTreeForwardIterator itr(instance->poActive->GetRoot());
		return itr;
	}

	PCSTreeReverseIterator LayerManager::GetReverseActiveIterator()
	{
		LayerManager* instance = LayerManager::privGetInstance();
		assert(instance);

		PCSTreeReverseIterator itr(instance->poActive->GetRoot());
		return itr;
	}

	void LayerManager::Attach()
	{
		LayerManager* pGOM = LayerManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poActive->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		Layer* pLayer = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pLayer = (Layer*)pIt.Current();
			pLayer->OnAttach();
		}
	}

	void LayerManager::Awake()
	{
		LayerManager* pGOM = LayerManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poActive->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		Layer* pLayer = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pLayer = (Layer*)pIt.Current();
			pLayer->Awake();
		}
	}

	void LayerManager::Start()
	{
		LayerManager* pGOM = LayerManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poActive->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		Layer* pLayer = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pLayer = (Layer*)pIt.Current();
			pLayer->Start();
		}
	}

	void LayerManager::Update(float deltaTime)
	{
		LayerManager* pGOM = LayerManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poActive->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		Layer* pLayer = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pLayer = (Layer*)pIt.Current();
			pLayer->OnUpdate(deltaTime);
		}
	}

	void LayerManager::Render(float deltaTime)
	{
		LayerManager* pGOM = LayerManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poActive->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		Layer* pLayer = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pLayer = (Layer*)pIt.Current();
			pLayer->OnRender(deltaTime);
		}
	}

	void LayerManager::EndApplication()
	{
		LayerManager* pGOM = LayerManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poActive->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		Layer* pLayer = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pLayer = (Layer*)pIt.Current();
			pLayer->EndApp();
		}
	}

	void LayerManager::RenderImGui()
	{
		LayerManager* pGOM = LayerManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poActive->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		Layer* pLayer = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pLayer = (Layer*)pIt.Current();
			pLayer->OnImGuiRender();
		}
	}

	PCSNode* LayerManager::derivedCreateNode()
	{
		return new Layer("Layer");
	}

	Layer* LayerManager::Find(const char* name)
	{
		LayerManager* pGOM = LayerManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poActive->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		Layer* layer = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			layer = (Layer*)pIt.Current();

			if (layer->CampareName(name) == PCSNode::Code::SUCCESS)
			{
				return layer;
			}
		}

		return nullptr;
	}

	PCSTree* LayerManager::GetActiveLayers()
	{
		LayerManager* pGOM = LayerManager::privGetInstance();

		if (pGOM == nullptr) return nullptr;

		return pGOM->poActive;
	}

	LayerManager::~LayerManager()
	{

	}

	//Should not call baseAdd
	PCSNode* LayerManager::baseAdd(PCSNode* parent)
	{
		assert(false);
		return nullptr;
	}

}