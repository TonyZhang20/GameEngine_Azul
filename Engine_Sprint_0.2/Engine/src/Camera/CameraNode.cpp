#include "CameraNode.h"
#include "StringThis.h"

namespace Azul
{
	CameraNode::CameraNode()
		: DLink(), pCamera(nullptr)
	{
		this->privClear();
	}

	CameraNode::~CameraNode()
	{
		this->privClear();
	}

	void CameraNode::Update(float deltaTime)
	{
		assert(this->pCamera);
		this->pCamera->updateCamera(deltaTime);

		//Vec3 CameraPos;
		//pCamera->getPos(CameraPos);

		//AZUL_UNUSED_VAR(CameraPos);
		//CameraPos.Print("Camera");

		//pCamera->getLookAt(CameraPos);
		//CameraPos.Print("LookAt");
	}

	void CameraNode::Set(Camera::Name name, Camera* _camera)
	{
		//assert(_camera);

		this->pCamera = _camera;
		if(pCamera)	this->pCamera->SetName(name);
	}

	void CameraNode::Set(Camera* pCam)
	{
		this->pCamera = pCam;
	}

	Camera* CameraNode::GetCamera()
	{
		return this->pCamera;
	}

	char* CameraNode::GetName()
	{
		char* pName = nullptr;
		if (this->pCamera)
		{
			pName = this->pCamera->GetName();
		}

		return pName;
	}

	void CameraNode::Wash()
	{
		this->privClear();
	}

	bool CameraNode::Compare(DLink* pTargetNode)
	{
		// This is used in ManBase.Find() 
		assert(pTargetNode);

		CameraNode* pDataB = (CameraNode*)pTargetNode;

		bool status = false;

		assert(pDataB->pCamera);
		assert(this->pCamera);

		if (this->pCamera->name == pDataB->pCamera->name)
		{
			status = true;
		}

		return status;
	}

	void CameraNode::Dump()
	{
		Trace::out("      MeshNode(%p)\n", this);

		// Data:
		if (this->pCamera)
		{
			Trace::out("      Mesh(%p) \n", this->pCamera);
			//TODO: String this -> Camera Name
			//Trace::out("      Name: %s \n", StringMe(this->poMesh->name));
		}
		else
		{
			Trace::out("      Name: %s \n", "null");
		}

		DLink::Dump();
	}

	void CameraNode::privClear()
	{
		if (this->pCamera)
		{
			delete this->pCamera;
		}
		this->pCamera = nullptr;
	}

}