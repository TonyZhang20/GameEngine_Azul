#include "GameObject_RotateAround.h"
#include "MathEngine.h"
#include "GraphicsObject_LightTexture.h"

namespace Azul
{
	GameObject_RotateAround::GameObject_RotateAround()
		: GameObject()
	{
	}

	GameObject_RotateAround::GameObject_RotateAround(GraphicsObject* poGraphicsObject)
		: GameObject(poGraphicsObject)
	{

	}

	GameObject_RotateAround::~GameObject_RotateAround()
	{

	}

	void GameObject_RotateAround::SetTarget(GameObject* target)
	{
		this->target = target;
		rotate = true;
	}


	void GameObject_RotateAround::Update(float deltaTime)
	{

		assert(rotate);
		assert(target != nullptr);

		angle += 0.02f; 

		Vec3 center = *(target->GetPos());
		Vec3* scale = target->GetScale();

	
		Rot rotY(Rot1::X, angle);

		float radius = 2.0f;

		float _x = center[x];
		float _z = center[z];
		float _y = center[y]; 

		switch (dir)
		{
		case Azul::Rot1::X:
			_y += radius * cosf(angle);
			_z += radius * sinf(angle);
			break;
		case Azul::Rot1::Y:
			_x += radius * cosf(angle);
			_z += radius * sinf(angle);
			break;
		case Azul::Rot1::Z:
			_x += radius * cosf(angle);
			_y += radius * sinf(angle);
			break;
		default:
			break;
		}

		Vec3 myPos(_x, _y, _z);
		this->SetPos(myPos);

		Trans T(this->poPos->x(), this->poPos->y(), this->poPos->z());
		Rot R(dir, angle);
		Scale S(scale->x(), scale->y(), scale->z());

		*this->poWorld = S * R * T;
		this->poGraphicsObject->SetWorld(*this->poWorld);

		if (this->colorChange == true)
		{
			GraphicsObject_LightTexture* go = (GraphicsObject_LightTexture*)this->poGraphicsObject;

			this->colorLerpT += deltaTime * colorChangeSpeed;

			if (this->colorLerpT >= 1.0f)
			{
				this->colorLerpT = 0.0f;
				
				Vec3 temp(this->orgin);

				this->orgin.set(targetC);
				this->targetC.set(temp);
			}


			go->poLightColor->set(this->orgin * (1.0f - this->colorLerpT) + this->targetC * this->colorLerpT);
		}
	}

	void GameObject_RotateAround::Draw()
	{
		this->poGraphicsObject->Render();
	}

	void GameObject_RotateAround::SetDir(Rot1 rot)
	{
		this->dir = rot;
	}
	void GameObject_RotateAround::SetLightColor(Vec4 originColor, Vec4 targetColor)
	{
		this->colorChange = true;
		this->orgin = originColor;
		this->targetC = targetColor;

		GraphicsObject_LightTexture* go = (GraphicsObject_LightTexture*)this->poGraphicsObject;

		go->poLightColor->set(this->orgin);
	}
}

