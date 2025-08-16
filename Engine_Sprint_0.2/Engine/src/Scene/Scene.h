#ifndef SCENE_H
#define SCENE_H

#include "ArchetypeWorld.h"
#include "Entity.h"

namespace Azul
{
	class Scene
	{
	public:
		friend class ZEntity;
		
		Scene();
		~Scene();
		virtual void Update(float deltaTime);

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

		ZEntity CreateEntity(const std::string& name = "");

	private:
		zecs::ArchetypeWorld world;
	};
}


#endif // !SCENE_H
