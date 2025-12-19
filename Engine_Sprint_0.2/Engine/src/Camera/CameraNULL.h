#ifndef  CAMERA_NULL_H
#define CAMERA_NULL_H

#include "Camera.h"

namespace Azul
{
	class CameraNULL final : public Camera
	{
	public:
		CameraNULL();
		CameraNULL(const CameraNULL&) = delete;
		CameraNULL& operator = (const CameraNULL&) = delete;
		~CameraNULL() = default;

		void updateCamera(float) override;
	};
}



#endif //  CAMERA_NULL_H
