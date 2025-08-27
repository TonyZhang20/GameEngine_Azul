#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "SceneManager.h"
#include "ZVector.hpp"

namespace Azul
{
	class Scene;
	
	class SceneManager
	{
	public:
		SceneManager() = default;

		static Scene* GetMainScene();

		static void AddScene(Scene*);
		static void Create();
		static void Destroy();

	private:
		ZVector<Scene*> sceneList;
		inline static SceneManager* instance = nullptr;
	};
}


#endif // !SCENEMANAGER_H
