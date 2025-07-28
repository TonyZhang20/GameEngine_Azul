//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "MathEngine.h"
#include "PCSNode.h"
#include "GraphicsObject.h"

namespace Azul
{

	class GameObject
	{
	public:
		GameObject();
		GameObject(GraphicsObject* poGraphicsObject);

		GameObject(const GameObject&) = delete;
		GameObject& operator = (const GameObject&) = delete;
		virtual ~GameObject();


		virtual void Update(float t);
		virtual void Draw();

		Mat4* GetWorld();

		void SetGraphicObject(GraphicsObject* poGraphicsObject);
		GraphicsObject* GetGraphicsObject();

		void SetPos(Vec3 v);
		void SetScale(float v);
		void SetScale(Vec3 scale);

		void SetWorld(Mat4* mat);

		Vec3* GetPos();
		Vec3* GetScale();


	protected:
		Vec3* poScale;
		Vec3* poPos;
		Mat4* poWorld;
		GraphicsObject* poGraphicsObject;

	//For Demo
	public:
		bool rotate = false;
	};

}


#endif


// --- End of File ---
