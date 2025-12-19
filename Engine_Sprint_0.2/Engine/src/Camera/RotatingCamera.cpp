#include "RotatingCamera.h"

namespace Azul
{
	RoatingCamera::RoatingCamera() : Camera()
	{

	}

	void RoatingCamera::updateCamera(float deltaTime)
	{
		Vec3 pos;
		Vec3 tar;
		Vec3 up;
		Vec3 upNorm;
		Vec3 forwardNorm;
		Vec3 rightNorm;

		GetHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

		pos = pos - 0.003f * rightNorm;

		SetHelper(up, tar, pos);

		Camera::updateCamera(deltaTime);
	}



}
