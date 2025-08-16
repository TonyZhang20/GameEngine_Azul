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

	ZEntity Scene::CreateEntity(const std::string& name)
	{
		ZEntity entity = ZEntity{ this };
		auto& tag = entity.AddComponent<TagComponent>();

		if (name.empty())
		{
			tag.SetName("Unnamed_GameObject");
		}

		return entity;
	}
}