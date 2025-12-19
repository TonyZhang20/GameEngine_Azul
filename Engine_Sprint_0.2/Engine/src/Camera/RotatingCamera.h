#ifndef ROTATE_CAMERA_H
#define ROTATE_CAMERA_H

#include "MathEngine.h"
#include "Camera.h"

namespace Azul
{
	class RoatingCamera : public Camera
	{
	public:
		RoatingCamera();
		RoatingCamera(const RoatingCamera&) = delete;
		RoatingCamera& operator = (const RoatingCamera&) = delete;
		virtual ~RoatingCamera() = default;

		virtual void updateCamera(float) override;

	public:
		float speed = 0.02f;

	private:
	};

}


#endif