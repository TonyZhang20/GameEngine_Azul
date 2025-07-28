//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAMEOBJECT_NODE_H
#define GAMEOBJECT_NODE_H

#include "PCSNode.h"
#include "GameObject.h"

namespace Azul
{

	class GameObjectNode : public PCSNode
	{
	public:
		GameObjectNode();
		GameObjectNode(const GameObject&) = delete;
		GameObjectNode& operator = (const GameObject&) = delete;
		virtual ~GameObjectNode();

		void Update(float deltaTime);
		void Draw();

		Vec3* GetPos();
		Vec3* GetScale();
		Mat4* GetWorld();

		void Set(const char* name, GameObject* pShaderObject);
		GameObject* GetGameObject();
		bool Compare(PCSNode* pTargetNode) override;

	private:
		void privClear();

		//--------------------------------------------------
		// Data
		//--------------------------------------------------
		GameObject* pGameObject;
	};

}


#endif

// --- End of File ---
