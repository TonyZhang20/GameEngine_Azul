//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObject.h"
#include "Game.h"
#include "Camera.h"

namespace Azul
{
	// HACK
	extern Game* poGame;
	extern Camera GameCamera;

	GameObject::GameObject() : poGraphicsObject(nullptr),
		poPos(new Vec3(0, 0, 0)),
		poScale(new Vec3(1 ,1 ,1)),
		poWorld(new Mat4(Identity))
	{
		assert(poPos);
		assert(this->poWorld);
	}

	GameObject::GameObject(GraphicsObject* pGraphicsObject)
		: poGraphicsObject(pGraphicsObject),
		poPos(new Vec3(0, 0, 0)),
		poScale(new Vec3(1, 1, 1)),
		poWorld(new Mat4(Identity))
	{
		assert(pGraphicsObject);
		assert(poPos);
		assert(this->poWorld);
	}

	GameObject::~GameObject()
	{
		delete this->poPos;
		delete this->poWorld;
		delete this->poGraphicsObject;
	}

	Mat4* GameObject::GetWorld()
	{
		return this->poWorld;
	}

	void GameObject::SetGraphicObject(GraphicsObject* _poGraphicsObject)
	{
		this->poGraphicsObject = _poGraphicsObject;
	}

	GraphicsObject* GameObject::GetGraphicsObject()
	{
		return this->poGraphicsObject;
	}

	void GameObject::SetPos(Vec3 v)
	{
		poPos->set(v);
	}

	void GameObject::SetScale(float v)
	{
		this->poScale->set(*this->poScale * v);
	}

	void GameObject::SetScale(Vec3 scale)
	{
		this->poScale->set(*this->poScale);
	}

	void GameObject::SetWorld(Mat4* mat)
	{
		this->poWorld = mat;
	}

	Vec3* GameObject::GetPos()
	{
		return this->poPos;
	}

	Vec3* GameObject::GetScale()
	{
		return this->poScale;
	}

	void GameObject::Update(float)
	{
		static float angle = 0.0f;
		//angle += 0.01f;

		Trans TransA(poPos->x(), poPos->y(), poPos->z());
		//Rot RotA(Rot1::Z, angle);
		Scale scale(poScale->x(), poScale->y(), poScale->z());

		*this->poWorld = scale * TransA;

		// PUSH to Graphics object
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void GameObject::Draw()
	{
		// ---------------------------------------------
		//  Transfer data to the constant buffer
		//    CPU ---> GPU
		//    World, View, Projection Matrix
		// ---------------------------------------------
		this->poGraphicsObject->Render();
	}
}

// --- End of File ---
