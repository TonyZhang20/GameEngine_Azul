#include "Scene.h"
#include "ZEntity.h"
#include "Components.h"
namespace Azul
{
	Scene::Scene()
	{
	
	}

	Scene::~Scene()
	{
	
	}

	void Scene::Update(float deltaTime)
	{

	}

	ZEntity Scene::CreateEntity(const char* name)
	{
		ZEntity entity = ZEntity{ this };
		TagComponent& tag = entity.AddComponent<TagComponent>();

		if (name == "")
		{
			tag.SetName("Unnamed_GameObject");
		}

		return entity;
	}
}