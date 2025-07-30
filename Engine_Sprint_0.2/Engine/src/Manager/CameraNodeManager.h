#ifndef CAMERA_MAN_H
#define CAMERA_MAN_H 

#include "ManBase.h"
#include "Camera.h"
#include "CameraNode.h"

namespace Azul
{
	class CameraNodeManager : public ManBase
	{
		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------
	public:
		static void Create(int reserveNum = 0, int reserveGrow = 1);
		static void Destroy();

		static CameraNode* Add(Camera::Name name, Camera* pMesh);
		static CameraNode* Find(Camera::Name name);

		static Camera* GetMainCam();
		static void SetMainCam(Camera* cam);

		static void Remove(CameraNode* pNode);
		static void Dump();

		static void UpdateCamera();
		static void UpdateAspectRatio(float ratio);

		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static CameraNodeManager* privGetInstance();

		~CameraNodeManager();
		CameraNodeManager(int reserveNum, int reserveGrow);

		//----------------------------------------------------------------------
		// Override Abstract methods
		//----------------------------------------------------------------------
	protected:
		DLink* derivedCreateNode() override;

		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		CameraNode* poNodeCompare;
		CameraNode* poMainCam;
		static CameraNodeManager* posInstance;

	};

}



#endif