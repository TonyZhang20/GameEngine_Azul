#include "RotateAroundCamera.h"

namespace Azul
{
	RotateAroundCamera::RotateAroundCamera() : Camera()
	{
	}
	void RotateAroundCamera::updateCamera(float deltaTime)
	{
		Vec3 pos;
		Vec3 tar;
		Vec3 up;
		Vec3 upNorm;
		Vec3 forwardNorm;
		Vec3 rightNorm;

		GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		Vec3 targetPos = Vec3(10, 0, 0);

		angle += 0.01f;
		float radius = 10.f;

		float _x = targetPos[x] + radius * cosf(angle);
		float _z = targetPos[z] + radius * sinf(angle);
		float _y = pos[y];

		Vec3 newPos(_x, 0, _z);

		Vec3 forward = targetPos - newPos;
		forward.norm();

		Vec3 right = up.cross(targetPos);
		right.norm();

		up.set(0, 1, 0);

		this->setOrientAndPosition(up, targetPos, newPos);


		Camera::updateCamera(deltaTime);
	}

}

