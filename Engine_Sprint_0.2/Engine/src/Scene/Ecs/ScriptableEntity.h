#ifndef SCRIPTABLE_ENTITY_H
#define SCRIPTABLE_ENTITY_H

#include "ZEntity.h"

namespace Azul
{
	class ScriptableEntity
	{
	public:

		virtual ~ScriptableEntity() {};

		template<typename T>
		inline T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		
		virtual void OnUpdate(float deltaTime) {};

	private:
		ZEntity m_Entity;
		friend class Scene;
	};
}


#endif // !SCRIPTABLE_ENTITY_H
