#ifndef SCENE_H
#define SCENE_H

#include "ArchetypeWorld.h"
#include "Entity.h"
#include "ZVector.hpp"

namespace Azul
{
	class ZEntity;
	class SceneHierachyPanel;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void OnInit();
		virtual void Update(float deltaTime);
		virtual void OnDestroy();

		template<typename T>
		ZVector<T> FindObjectsBy()
		{
			ZVector<zecs::EntityID> eids = this->world.Query<T>();

			ZVector<T> components;
			
			for (auto id : eids)
			{
				T& Component = this->world.GetComponent<T>(id);
				components.push_back(Component);
			}

			return components;
		}

		template<typename... Components>
		ZVector<ZEntity> Query()
		{
			auto entities = this->world.Query<Components...>();
			
			ZVector<ZEntity> result;

			for (zecs::EntityID id : entities)
			{
				result.emplace_back(ZEntity{id, this});
			}

			return result;
		}

		ZEntity CreateEntity(const char* name = "");
		void DestroyEntity(zecs::EntityID entity);

	protected:
		void UpdateScript(float d);

	protected:
		friend ZEntity;
		friend SceneHierachyPanel;

		zecs::ArchetypeWorld world;

		std::string m_name;
	};
}


#endif // !SCENE_H
