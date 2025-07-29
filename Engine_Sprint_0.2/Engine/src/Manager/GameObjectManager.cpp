#include "Mesh.h"
#include "MeshNull.h"
#include "GraphicsObject_Null.h"
#include "ShaderObject_Null.h"
#include "GameObject.h"
#include "MeshNodeManager.h"
#include "GameObject_Null.h"
#include "GameObjectManager.h"
#include "PCSNode.h"
#include "PCSTreeForwardIterator.h"

namespace Azul
{
	GameObjectManager* GameObjectManager::instance = nullptr;

	void GameObjectManager::Create(int _reserveNum, int _reserveGrow)
	{

		assert(_reserveNum >= 0);
		assert(_reserveGrow > 0);

		if (instance == nullptr)
		{
			GameObjectManager::instance = new GameObjectManager(_reserveNum, _reserveGrow);
		}
		else
		{
			assert(false);
		}

		GameObject_NULL* pGameRoot = new GameObject_NULL();

		GameObjectManager* pMan = GameObjectManager::instance;
		GameObjectNode* rootNode = (GameObjectNode*)pMan->baseAdd(pMan->poActive->GetRoot());

		rootNode->Set("GameObject_Root", pGameRoot);

		assert(instance);
	}

	GameObjectManager::GameObjectManager(int _reserveNum, int _reserveGrow)
		: PCSManBase(new PCSTree(), new PCSTree(), _reserveNum, _reserveGrow)
	{

	}

	GameObjectNode* GameObjectManager::Add(const char* name, GameObject* node, GameObjectNode* parent)
	{
		// Get singleton
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		GameObjectNode* objNode = (GameObjectNode*)pGOM->baseAdd(parent);

		assert(objNode);
		objNode->Set(name, node);

		return objNode;
	}

	void GameObjectManager::Destroy()
	{
		// Get singleton
		Trace::out("\n");
		Trace::out("--- GameObjectMan::Destroy() ----\n");

		//pGOM->baseDestroy();

		if (GameObjectManager::instance != nullptr)
		{
			delete GameObjectManager::instance;
			GameObjectManager::instance = nullptr;
		}

		Trace::out("------		Finished	 ------\n");
		Trace::out("\n");
	}

	void GameObjectManager::Update(float deltaTime)
	{
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poActive->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		GameObjectNode* pGameObj = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pGameObj = (GameObjectNode*)pIt.Current();
			pGameObj->Update(deltaTime);
		}
	}

	void GameObjectManager::Draw()
	{
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poActive->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		GameObjectNode* pGameObj = nullptr;

		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pGameObj = (GameObjectNode*)pIt.Current();
			pGameObj->Draw();
		}
	}

	PCSNode* GameObjectManager::derivedCreateNode()
	{
		return new GameObjectNode();
	}

	GameObject* GameObjectManager::Find(const char* name)
	{
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		assert(pGOM);

		PCSNode* pRootNode = pGOM->poActive->GetRoot();
		assert(pRootNode);

		PCSTreeForwardIterator pIt(pRootNode);

		GameObjectNode* pGameObj = nullptr;


		for (pIt.First(); !pIt.IsDone(); pIt.Next())
		{
			pGameObj = (GameObjectNode*)pIt.Current();
			
			if(pGameObj->CampareName(name) == PCSNode::Code::SUCCESS)
			{
				return pGameObj->GetGameObject();
			}
		}
	}

	GameObjectManager::~GameObjectManager()
	{
		//delete this->poActive;
	}

	PCSTree* GameObjectManager::GetActiveGameObjects()
	{
		GameObjectManager* pGOM = GameObjectManager::privGetInstance();
		
		if (pGOM == nullptr) return nullptr;

		return pGOM->poActive;
	}

	GameObjectManager* GameObjectManager::privGetInstance(void)
	{
		// This is where its actually stored (BSS section)
		//assert(instance != nullptr);

		return instance;
	}
}



// --- End of File ---
