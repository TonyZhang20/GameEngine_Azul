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
		static void RemoveScene(Scene*);

		static void Create();
		static void Destroy();
		static void Init();
		static void Update(float deltaTime);

	private:
		static SceneManager* GetInstance();

	private:
		ZVector<Scene*> sceneList;
		inline static SceneManager* instance = nullptr;
	};
}


#endif // !SCENEMANAGER_H
