#ifndef ENTITY_H_AZUL
#define ENTITY_H_AZUL


#include "Entity.h"
#include "Scene.h"

namespace Azul
{
	class ZEntity
	{
	public:
		ZEntity();
		ZEntity(Scene* scene);
		ZEntity(zecs::EntityID id, Scene* scene);
		ZEntity(const ZEntity& otehr) = default;
		ZEntity(ZEntity&&) = default;
		ZEntity& operator=(const ZEntity& other);

		template<typename T>
		inline T& AddComponent(T&& components)
		{
			assert(!HasComponent<T>());
			return this->parentScene->world.AddComponent<T>(this->m_EntityHandle, std::forward<T>(components));
		}

		template<typename T>
		inline T& AddComponent()
		{
			assert(!HasComponent<T>());
			return this->parentScene->world.AddComponent<T>(this->m_EntityHandle, T{});
		}

		template<typename T>
		inline T& AddComponent(T& t)
		{
			assert(!HasComponent<T>());
			return this->parentScene->world.AddComponent<T>(this->m_EntityHandle, t);
		}

		template<typename T>
		inline T& GetComponent()
		{
			assert(HasComponent<T>());
			return parentScene->world.GetComponent<T>(this->m_EntityHandle);
		}

		template<typename T>
		inline void RemoveComponent()
		{
			assert(HasComponent<T>());
			return parentScene->world.RemoveComponent<T>(this->m_EntityHandle);
		}

		template<typename T>
		inline bool HasComponent()
		{
			return parentScene->world.HasComponent<T>(this->m_EntityHandle);
		}

		~ZEntity();

		operator zecs::EntityID() const { return this->m_EntityHandle; }

	private:
		zecs::EntityID m_EntityHandle;
		Scene* parentScene;
	};
}



#endif // ENTITY_H
