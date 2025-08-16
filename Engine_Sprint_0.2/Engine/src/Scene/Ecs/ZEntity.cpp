#include "ZEntity.h"
#include "Components.h"
namespace Azul
{
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

	ZEntity::~ZEntity()
	{
	
	}
}