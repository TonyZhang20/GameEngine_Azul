#ifndef ROTATE_AROUND_CAMERA_H
#define ROTATE_AROUND_CAMERA_H

#include "MathEngine.h"
#include "Camera.h"
#include "GameObject.h"

namespace Azul
{
	class RotateAroundCamera : public Camera
	{
	public:
		RotateAroundCamera();
		RotateAroundCamera(const RotateAroundCamera&) = delete;
		RotateAroundCamera& operator = (const RotateAroundCamera&) = delete;
		virtual ~RotateAroundCamera() = default;

		virtual void updateCamera(float) override;

	public:
		float speed = 0.02f;
		float radius = 5.f;
		float angle = 0.0f;
		GameObject* target;

	private:
	};

}


#endif