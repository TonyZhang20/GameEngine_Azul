//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef GAME_H
#define GAME_H

#include "ZEntity.h"
#include "Engine.h"
#include "MathEngine.h"
#include "Components.h"
#include "ArchetypeWorld.h"
#include "MouseEvent.h"

namespace Azul
{
	class Game : public Engine
	{
	public:
		Game();
		Game(const Game&) = delete;
		Game& operator = (const Game&) = delete;
		virtual ~Game();
		
		virtual bool LoadContent() override;
		virtual void UnloadContent() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() override;
		virtual	void ClearDepthStencilBuffer() override;
		virtual void OnDetach() override;

		float GetAspectRatio() const;
		void SetDefaultTargetMode();

		virtual void CollectRenderPackets();
		virtual ZVector<RenderPacket>& GetRenderPacket();

		ZEntity& GetMainCam();

		//Event
	protected:
		bool OnWindowResizeEvent(WindowResizeEvent& e) override;
		bool OnMouseMovedEvent(MouseMovedEvent& e) override;

	private:
		ZVector<RenderPacket> outPacks;
		ZVector<ZEntity> renderableEntity;

		zecs::ArchetypeWorld world;

		class MainScene* mainScene;
		ZEntity camera;
	};
}


#endif
// --- End of File ---

