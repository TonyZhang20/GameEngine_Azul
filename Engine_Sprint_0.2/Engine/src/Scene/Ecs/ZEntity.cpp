#include "ZEntity.h"
#include "Scene.h"
#include "Components.h"

namespace Azul
{
	ZEntity::ZEntity()
	{
		this->m_EntityHandle = zecs::EntityID{ 0xffffff, 0xff };
		parentScene = nullptr;
	}

	ZEntity::ZEntity(Scene* scene)
	{
		this->parentScene = scene;

		this->m_EntityHandle = parentScene->world.SpawnEntity<TransformComponent>(TransformComponent{});
	}

	ZEntity::ZEntity(zecs::EntityID id, Scene* scene)
	{
		this->m_EntityHandle = id;
		this->parentScene = scene;
	}

	ZEntity& ZEntity::operator=(const ZEntity& other)
	{
		this->m_EntityHandle = other.m_EntityHandle;
		this->parentScene = other.parentScene;
		return *this;
	}

	ZEntity::~ZEntity()
	{
	
	}
}