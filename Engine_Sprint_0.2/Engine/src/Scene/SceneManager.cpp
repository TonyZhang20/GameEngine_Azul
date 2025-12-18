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
		GetInstance()->sceneList.push_back(s);
	}

	void SceneManager::RemoveScene(Scene* s)
	{
		GetInstance()->sceneList.erase(s);
	}

	void SceneManager::Init()
	{
		for (Scene* scene : GetInstance()->sceneList)
		{
			scene->OnInit();
		}
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
		for (Scene* scene : GetInstance()->sceneList)
		{
			scene->OnDestroy();
			delete scene;
		}

		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

	SceneManager* SceneManager::GetInstance()
	{
		if(instance == nullptr)
		{
			Create();
		}

		return SceneManager::instance;
	}
}