#ifndef GAME_OBJECT_MAN_H
#define GAME_OBJECT_MAN_H

#include "GameObjectNode.h"
#include "PCSManBase.h"

namespace Azul
{

	// Singleton
	class GameObjectManager : public PCSManBase
	{
	public:
		static GameObjectNode* Add(const char* name, GameObject* node, GameObjectNode* parent = nullptr);
		static void Draw();
		static void Update(float deltaTime);

		static void Create(int _reserveNum = 3, int _reserveGrow = 1);
		static void Destroy();

		GameObjectManager(const GameObjectManager&) = delete;
		GameObjectManager& operator = (const GameObjectManager&) = delete;

		virtual PCSNode* derivedCreateNode();
		static GameObject* Find(const char* name);

		~GameObjectManager();

		static PCSTree* GetActiveGameObjects();

	private:
		GameObjectManager(int _reserveNum = 3, int _reserveGrow = 1);
		static GameObjectManager* privGetInstance();

		// data
		//PCSTree* poRootTree;
		static GameObjectManager* instance;
	};

}

#endif


// --- End of File ---
