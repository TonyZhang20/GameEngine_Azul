#ifndef SCENE_HIERACHY_PANEL_H
#define SCENE_HIERACHY_PANEL_H

#include "Scene.h"

namespace Azul
{
	class SceneHierachyPanel
	{
	public:
		SceneHierachyPanel(Scene* scene);
		void SetContext(Scene* scene);

		void OnImGuiRender();
		void DrawEntityNode(class zecs::EntityID entity);
		void DrawComponents(zecs::EntityID entity);

		static void DrawVec3Control(const char* label, class Vec3& values, float speed = 1.0f, float resetValue = 0.0f, float column = 100.0f);
	private:
		Scene* mContext;
		zecs::EntityID m_SelectionContext;
	};
}


#endif // !SCENE_HIERACHY_PANEL_H
