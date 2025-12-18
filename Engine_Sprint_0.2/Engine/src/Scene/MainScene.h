#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "Scene.h"
#include "ZEntity.h"

namespace Azul
{
	class MainScene : public Scene
	{
	public:
		MainScene();
		MainScene(const MainScene&) = delete;
		MainScene& operator=(const MainScene&) = delete;
		~MainScene() override;

		inline ZEntity& GetMainCamera() { return this->editorCamera; }

		virtual void OnInit() override;
		virtual void Update(float deltaTime) override;
		virtual void OnDestroy() override;

	private:
		ZEntity editorCamera;

	};
}

#endif // !
