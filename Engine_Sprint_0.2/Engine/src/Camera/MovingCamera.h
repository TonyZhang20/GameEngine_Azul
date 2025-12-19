#ifndef MOVING_CAMERA_H
#define MOVING_CAMERA_H

#include "MathEngine.h"
#include "Camera.h"

namespace Azul
{
	class MovingCamera : public Camera
	{
	public:
		MovingCamera(Vec3 origin, Vec3 target);
		MovingCamera(const MovingCamera&) = delete;
		MovingCamera& operator = (const MovingCamera&) = delete;
		virtual ~MovingCamera() = default;

		virtual void updateCamera(float) override;

	public:
		float speed = 0.02f;

	private:
		Vec3 _origin;
		Vec3 _target;
		bool movingRight = true;
	};

}


#endif