#include "SceneManager.h"
#include "Scene.h"

namespace Azul
{
	Scene* SceneManager::GetMainScene()
	{
		if (instance && !instance->sceneList.empty())
		{
			return instance->sceneList[0];
		}
		return nullptr;
	}

	void SceneManager::AddScene(Scene* s)
	{
		if (!instance) Create();
		instance->sceneList.push_back(s);
	}

	void SceneManager::Create()
	{
		if (instance == nullptr)
		{
			instance = new SceneManager();
		}
	}

	void SceneManager::Destroy()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

}