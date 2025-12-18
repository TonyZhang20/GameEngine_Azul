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
		Trace::out("Scene Destroyed\n");
	}

	void Scene::OnInit()
	{

	}

	void Scene::Update(float deltaTime)
	{
		UpdateScript(deltaTime);
	}

	void Scene::OnDestroy()
	{

	}

	ZEntity Scene::CreateEntity(const char* name)
	{
		ZEntity entity = ZEntity{ this };
		TagComponent& tag = entity.AddComponent<TagComponent>();

		if (name == NULL || name[0] == '\0')
		{
			tag.SetName("Unnamed_GameObject");
		}
		else
		{
			tag.SetName(name);
		}

		return entity;
	}

	void Scene::DestroyEntity(zecs::EntityID entity)
	{
		this->world.DestroyEntity(entity);
	}

	void Scene::UpdateScript(float deltaTime)
	{
		auto Entitys = this->world.Query<NativeScriptComponent>();

		for (auto id : Entitys)
		{
			auto& Script = world.GetComponent<NativeScriptComponent>(id);

			if (!Script.Instance)
			{
				Script.Instance = Script.InstantiateScript();
				Script.Instance->m_Entity = ZEntity{ id, this };
				Script.Instance->OnCreate();
			}

			Script.Instance->OnUpdate(deltaTime);
		}
	}
}