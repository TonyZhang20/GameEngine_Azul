#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

#include "DLink.h"
#include "Camera.h"

namespace Azul
{
	class CameraNode : public DLink
	{
	public:
		CameraNode();
		CameraNode(const Camera&) = delete;
		CameraNode& operator = (const Camera&) = delete;
		virtual ~CameraNode();

		void Update(float deltaTime);
		void Set(Camera::Name name, Camera* pCam);
		void Set(Camera* pCam);
		Camera* GetCamera();


		virtual char* GetName() override;
		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;

	private:
		void privClear();

	private:
		Camera* pCamera;
	};
}



#endif